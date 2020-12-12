# Copyright (C) 2020 luna_koly


cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    CashProject
        VERSION 1.1
        DESCRIPTION "The interpreter I've always wanted?"
        LANGUAGES CXX
)

add_subdirectory("sources")

target_include_directories(
    Cash PUBLIC
        "sources"
        "${PROJECT_BINARY_DIR}/sources/main"
)
