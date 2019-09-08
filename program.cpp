#include "mysql.h"
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
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


MYSQL *InitMysql(char const * dllPath)
{
    HINSTANCE hGetProcIDDLL = LoadLibrary(dllPath);

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

int main(int argc, char *argv[])
{
    OdbcMain("Driver={SQL Server Native Client 11.0}; Server=iq15r2; Database=KMS_FZ;Uid=ECUser;Pwd=ECPassword;");

    MYSQL *m = InitMysql("libmysql.dll");
    if (!m)
    {
        std::cout << "init failed, reason: " << mysql::error(m) << std::endl;
        return EXIT_FAILURE;
    }
    
    auto result = mysql::real_connect(m, "localhost", "root", nullptr, "mysql", 55555, nullptr, CLIENT_FOUND_ROWS);
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
    
    auto const q = "SELECT * FROM db;";
    
    std::cout << std::endl << "Running query: " << std::endl << "    " << q << std::endl;
    if (mysql::query(m, q))
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
