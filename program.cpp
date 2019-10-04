#include "mysql.h"
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sys/stat.h>
#include "odbcsql.h"

void PrintResults(MYSQL *m, MYSQL_RES *resultset)
{
    if (resultset == nullptr) 
    {
        std::cerr << "failed to store result" << mysql::error(m) << std::endl;
        return;
    }

    int field_count = mysql::num_fields(resultset);

    MYSQL_ROW row;

    while ((row = mysql::fetch_row(resultset))) 
    {
        std::cout << "    ";
        for (int i = 0; i < field_count; i++) 
        {
            if (i > 0) std::cout << " - ";
            std::cout << (row[i] ? row[i] : "NULL"); 
        } 
        printf("\n"); 
    }
}

template <typename T>
T load_method(HINSTANCE dll, char const *name)
{
    auto method = (T)GetProcAddress(dll, name);
    if (!method)
    {
        std::cout << "could not locate the function " << name << std::endl;
        return nullptr;
    }
    
    return method;
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::map<std::string, std::string> parseKV(const std::string &sz)
{
    std::map<std::string, std::string> ret;
    std::string key;
    std::string value;
    char endDelim = ';';
    const char *s = sz.c_str();
    
    while(*s)
    {
        // skip all spaces
        while (*s && *s <= ' ')
        {
            s++;
        }
        
        // parse the key
        while(*s && *s != '=')
        {
            key.push_back(*s);
            ++s;
        }
        
        // if we quit due to the end of the string exit now
        if(!*s) break;
        
        // skip the "=" and all spaces
        while (*s && (*s == '=' || *s <= ' '))
        {
            s++;
        }
        
        if (!*s) break;
        
        if (*s == '\'')
        {
            endDelim = '\'';
        }
        
        // parse the value
        while(*s && *s != endDelim)
        {
            value.push_back(*s);
            ++s;
        }
        
        // skip the ";", the end delimiter and all spaces
        while (*s && (*s == endDelim || *s == ';' || *s <= ' '))
        {
            s++;
        }
        
        trim(key);
        trim(value);
        
        ret[key] = value;
        key.clear();
        value.clear();
        endDelim = ';';
    }
    return ret;
}

MYSQL *InitMysql()
{
    HINSTANCE hGetProcIDDLL = LoadLibrary("libmysql.dll");

    if (!hGetProcIDDLL)
    {
        hGetProcIDDLL = LoadLibrary("libmariadb.dll");
    }

    if (!hGetProcIDDLL)
    {
        std::cout << "could not load the dynamic library" << std::endl;
        return nullptr;
    }

    mysql::init = load_method<FP_MYSQL_INIT>(hGetProcIDDLL, "mysql_init");
    mysql::get_client_version = load_method<FP_MYSQL_GET_CLIENT_VERSION>(hGetProcIDDLL, "mysql_get_client_version");
    mysql::get_server_version = load_method<FP_MYSQL_GET_SERVER_VERSION>(hGetProcIDDLL, "mysql_get_server_version");
    mysql::real_connect = load_method<FP_MYSQL_REAL_CONNECT>(hGetProcIDDLL, "mysql_real_connect");
    mysql::close = load_method<FP_MYSQL_CLOSE>(hGetProcIDDLL, "mysql_close");
    mysql::error = load_method<FP_MYSQL_ERROR>(hGetProcIDDLL, "mysql_error");
    mysql::select_db = load_method<FP_MYSQL_SELECT_DB>(hGetProcIDDLL, "mysql_select_db");
    mysql::list_dbs = load_method<FP_MYSQL_LIST_DBS>(hGetProcIDDLL, "mysql_list_dbs");
    mysql::list_tables = load_method<FP_MYSQL_LIST_TABLES>(hGetProcIDDLL, "mysql_list_tables");
    mysql::list_fields = load_method<FP_MYSQL_LIST_FIELDS>(hGetProcIDDLL, "mysql_list_fields");
    mysql::query = load_method<FP_MYSQL_QUERY>(hGetProcIDDLL, "mysql_query");
    mysql::store_result = load_method<FP_MYSQL_STORE_RESULT>(hGetProcIDDLL, "mysql_store_result");
    mysql::free_result = load_method<FP_MYSQL_FREE_RESULT>(hGetProcIDDLL, "mysql_free_result");
    mysql::num_fields = load_method<FP_MYSQL_NUM_FIELDS>(hGetProcIDDLL, "mysql_num_fields");
    mysql::fetch_row = load_method<FP_MYSQL_FETCH_ROW>(hGetProcIDDLL, "mysql_fetch_row");

    if (!(mysql::init && mysql::get_client_version && mysql::get_server_version && mysql::real_connect && mysql::close && mysql::error && mysql::select_db && mysql::list_dbs && mysql::list_tables && mysql::list_fields && mysql::query && mysql::store_result && mysql::free_result && mysql::num_fields && mysql::fetch_row))
    {
        return nullptr;
    }

    return mysql::init(0);
}

std::string saveCout(char const *str)
{
    if (str == nullptr)
    {
        return "";
    }
    
    return std::string(str);
}

int MysqlMain(char const *connectionString, char const *query)
{
    std::cout << "ConnectionString = " << connectionString << std::endl;
    std::cout << "Query = " << query << std::endl;
    
    auto config = parseKV(connectionString);
    
    char const *server = nullptr;
    char const *database = nullptr;
    char const *uid = nullptr;
    char const *pwd = nullptr;
    int portNumber = 3306;

    if (config.count("Server") >= 1)
    {
        server = config["Server"].c_str();
    }

    if (config.count("Database") >= 1)
    {
        database = config["Database"].c_str();
    }

    if (config.count("Uid") >= 1)
    {
        uid = config["Uid"].c_str();
    }

    if (config.count("Pwd") >= 1)
    {
        pwd = config["Pwd"].c_str();
    }

    if (config.count("Port") >= 1)
    {
        portNumber = atoi(config["Port"].c_str());
    }
    
    MYSQL *m = InitMysql();
    if (!m)
    {
        std::cout << "init failed, reason: " << mysql::error(m) << std::endl;
        return EXIT_FAILURE;
    }
    
    auto result = mysql::real_connect(m, server, uid, pwd, database, portNumber, nullptr, CLIENT_FOUND_ROWS);
    if (!result)
    {
        std::cout << "not connected, reason: " << mysql::error(m) << std::endl;
        return EXIT_FAILURE;
    }
    
    auto dbs = mysql::list_dbs(m, nullptr);
    
    std::cout << std::endl << "Databases:" << std::endl;
    PrintResults(m, dbs);
    
    mysql::free_result(dbs);
    
    auto tables = mysql::list_tables(m, nullptr);
    
    std::cout << std::endl << "Tables:" << std::endl;
    PrintResults(m, tables);
    
    mysql::free_result(tables);
    
    std::cout << std::endl << "Running query: " << std::endl << "    " << query << std::endl;
    if (mysql::query(m, query))
    {
        std::cerr << "query failed for reason: " << mysql::error(m) << std::endl;
    }
    
    MYSQL_RES *resultset = mysql::store_result(m);
    
    std::cout << std::endl << "Rows:" << std::endl;
    PrintResults(m, resultset);
    
    mysql::free_result(resultset);
    
    mysql::close(m);
    
    return 0;
}

#include <sqlite3.h>
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for(int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    
    return 0;
}

int SqliteMain(char const *connectionString, char const *query)
{
    std::cout << "ConnectionString = " << connectionString << std::endl;
    std::cout << "Query = " << query << std::endl;
    
    auto config = parseKV(connectionString);
    
    char const *dataSource = nullptr;
    
    if (config.count("Data Source") >= 1)
    {
        dataSource = config["Data Source"].c_str();
    }
    
    for (auto p : config)
    {
        std::cout << p.first << "=" << p.second << std::endl;
    }
    std::cout << "Opening Data Source: " << dataSource << std::endl;
    
    sqlite3 *db;
    int rc = sqlite3_open(dataSource, &db);
    
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return(1);
    }
    
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    
    sqlite3_close(db);
    
    return 0;
}

inline bool exists(char const *name)
{
    struct stat buffer;
    return (stat (name, &buffer) == 0); 
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "No file given" << std::endl;
        return EXIT_SUCCESS;
    }
    
    if (!exists(argv[1]))
    {
        std::cout << "File does not exist: " << argv[1] << std::endl;
        return EXIT_SUCCESS;
    }
    
    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    
    trim(str);
    
    if (str.size() == 0)
    {
        std::cout << "File is empty: " << argv[1] << std::endl;
        return EXIT_SUCCESS;
    }
    
    std::stringstream ss(str.c_str());
    std::string to;
    
    if (!std::getline(ss, to, '\n'))
    {
        std::cout << "File is empty: " << argv[1] << std::endl;
        return EXIT_SUCCESS;
    }
    
    auto query = str.substr(to.size());
    
    if (to.rfind("MYSQL ", 0) == 0)
    {
        MysqlMain(to.substr(6).c_str(), query.c_str());
    }
    else if (to.rfind("MSQL ", 0) == 0)
    {
        OdbcMain(to.substr(5).c_str()/* , query.c_str() */);
    }
    else if (to.rfind("SQLITE ", 0) == 0)
    {
        SqliteMain(to.substr(7).c_str(), query.c_str());
    }
    
    return 0;
}
