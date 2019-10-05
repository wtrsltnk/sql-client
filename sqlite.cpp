#include "sqlite.h"
#include "common.h"

#include <sqlite3.h>
#include <iostream>

static int callback(void *, int argc, char **argv, char **azColName)
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

    sqlite3 *db;
    int rc = sqlite3_open(dataSource, &db);

    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return(1);
    }

    char *zErrMsg = nullptr;
    rc = sqlite3_exec(db, query, callback, nullptr, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);

    return 0;
}
