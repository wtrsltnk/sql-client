cmake_minimum_required(VERSION 3.15)

add_library(sqlite
    sqlite3.c
    sqlite3.h
    sqlite3ext.h
    )

target_include_directories(sqlite
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
