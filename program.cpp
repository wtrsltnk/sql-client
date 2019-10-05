#include "common.h"
#include "mysql.h"
#include "odbcsql.h"
#include "sqlite.h"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cerr << "No file given" << std::endl;
        return EXIT_SUCCESS;
    }
    
    if (!exists(argv[1]))
    {
        std::cerr << "File does not exist: " << argv[1] << std::endl;
        return EXIT_SUCCESS;
    }
    
    std::ifstream t(argv[1]);
    std::string fileContent((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    
    trim(fileContent);
    
    if (fileContent.size() == 0)
    {
        std::cerr << "File is empty: " << argv[1] << std::endl;
        return EXIT_SUCCESS;
    }
    
    std::stringstream ss(fileContent.c_str());
    std::string connectionString;
    
    if (!std::getline(ss, connectionString, '\n'))
    {
        std::cerr << "File does not contain a connection string: " << argv[1] << std::endl;
        return EXIT_SUCCESS;
    }
    
    auto query = fileContent.substr(connectionString.size());
    
    if (connectionString.rfind("MYSQL ", 0) == 0)
    {
        MysqlMain(connectionString.substr(6).c_str(), query.c_str());
    }
    else if (connectionString.rfind("MSQL ", 0) == 0)
    {
        OdbcMain(connectionString.substr(5).c_str(), query.c_str());
    }
    else if (connectionString.rfind("SQLITE ", 0) == 0)
    {
        SqliteMain(connectionString.substr(7).c_str(), query.c_str());
    }
    else
    {
        std::cerr << "No supported connection string detected: " << connectionString << std::endl;
    }
    
    return 0;
}
