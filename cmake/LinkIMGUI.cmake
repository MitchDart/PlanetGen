include(FetchContent)

macro(LinkIMGUI TARGET ACCESS)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.83
    )

    FetchContent_GetProperties(imgui)

    if (NOT imgui_POPULATED)
        FetchContent_Populate(imgui)

        file(GLOB imgui_sources ${imgui_SOURCE_DIR}/**.cpp)
        SET(imgui_src ${imgui_sources})  
        
        add_library(imgui STATIC EXCLUDE_FROM_ALL ${imgui_src})
    endif()

    target_include_directories(${TARGET} ${ACCESS} ${imgui_SOURCE_DIR})
    IF (WIN32)
        target_link_libraries(${TARGET} ${ACCESS} imgui imm32)
    ELSE()
        target_link_libraries(${TARGET} ${ACCESS} imgui)
    ENDIF()
endmacro()
