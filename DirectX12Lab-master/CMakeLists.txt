cmake_minimum_required(VERSION 3.31)
project(DirectX12CustomLib LANGUAGES CXX)

# add_executable(DirectX12CustomLib)
add_executable(DirectX12CustomLib WIN32)

# Modern C++
set_target_properties(DirectX12CustomLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# MSVC warnings + utf-8
if (MSVC)
    target_compile_options(DirectX12CustomLib PRIVATE /W4 /permissive- /utf-8)
endif()

# Unicode (лучше так, чем add_definitions)
target_compile_definitions(DirectX12CustomLib PRIVATE UNICODE _UNICODE)

# --- 1) Источники: берём только из src/ (и при желании include/) ---
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)



target_sources(DirectX12CustomLib PRIVATE ${PROJECT_SOURCES})

# DirectX 12 libs (из Windows SDK)
target_link_libraries(DirectX12CustomLib PRIVATE
        d3d12 dxgi dxguid d3dcompiler winmm
)

# --- 2) Копирование content в папку билда рядом с exe ---
set(CONTENT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/content")
set(CONTENT_DST_DIR "$<TARGET_FILE_DIR:DirectX12CustomLib>/content")

add_custom_command(TARGET DirectX12CustomLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying content/ to build output directory"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CONTENT_DST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CONTENT_SRC_DIR}" "${CONTENT_DST_DIR}"
        VERBATIM
)