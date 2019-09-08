cmake_minimum_required(VERSION 3.15)

project(sql)

add_executable(sql
    program.cpp
    odbcsql.cpp
    )

target_link_libraries(sql
    PRIVATE odbc32
    )

target_compile_features(sql
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    )
