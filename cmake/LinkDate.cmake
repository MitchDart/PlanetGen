include(FetchContent)

macro(LinkDate TARGET ACCESS)
    FetchContent_Declare(
        date
        GIT_REPOSITORY https://github.com/HowardHinnant/date
        GIT_TAG v3.0.1
    )

    FetchContent_GetProperties(date)

    if (NOT date_POPULATED)
        FetchContent_Populate(date)

        add_subdirectory(${date_SOURCE_DIR} ${date_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    target_include_directories(${TARGET} ${ACCESS} ${date_SOURCE_DIR})
    target_include_directories(${TARGET} ${ACCESS} ${date_SOURCE_DIR}/include)
    target_link_libraries(${TARGET} ${ACCESS} date)

    add_dependencies(${TARGET} date)
endmacro()