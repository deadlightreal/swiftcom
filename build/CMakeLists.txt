cmake_minimum_required(VERSION 3.22)
project(main LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Minimum macOS version
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")

# Set wxWidgets include directories
set(wxWidgets_INCLUDE_DIRS 
    /opt/homebrew/lib/wx/include/osx_cocoa-unicode-3.3
    /opt/homebrew/Cellar/sqlite/3.50.4/include
    /opt/homebrew/include/cjson
    /opt/homebrew/include/wx-3.3
    /opt/homebrew/lib
    /opt/homebrew/Cellar/sqlite/3.50.4/lib
)

link_directories(/opt/homebrew/lib)

# Set wxWidgets libraries
set(wxWidgets_LIBRARIES
    wx_osx_cocoau_xrc-3.3
    wx_osx_cocoau_html-3.3
    wx_osx_cocoau_qa-3.3
    wx_osx_cocoau_core-3.3
    wx_baseu_xml-3.3
    wx_baseu_net-3.3
    wx_baseu-3.3
    cjson
    sqlite3
    /Users/admin/Desktop/raw_sock_network_example/external/lib/libswift_net.a
)

# Frameworks needed on macOS
set(MAC_FRAMEWORKS
    IOKit
    Carbon
    Cocoa
    QuartzCore
    AudioToolbox
    System
    OpenGL
)

# Collect all source files (scalable for many cpp files)
file(GLOB_RECURSE SOURCES "../src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})

# Link wxWidgets libraries and macOS frameworks
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
foreach(FRAMEWORK ${MAC_FRAMEWORKS})
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework ${FRAMEWORK}")
endforeach()

# Define preprocessor macros
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _FILE_OFFSET_BITS=64
    wxDEBUG_LEVEL=0
    WXUSINGDLL
    __WXMAC__
    __WXOSX__
    __WXOSX_COCOA__
)
