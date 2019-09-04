cmake_minimum_required(VERSION 3.15)

project(sql)

add_executable(sql
    program.cpp
    mysql.h
    )

target_compile_features(sql
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    )
