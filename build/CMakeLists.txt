cmake_minimum_required(VERSION 3.22)
project(main LANGUAGES CXX)

get_filename_component(EXTERNAL_DIRECTORY_LOCATION "../external" REALPATH)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Minimum macOS version
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")

link_directories(/opt/homebrew/lib)

set(INCLUDE_DIRS
    ../external/include/sqlite
    ../external/include/swift_net
    ../external/include/wx
    ../external/include/wx/macos/osx_cocoa-unicode-3.3
)

# Set wxWidgets libraries
set(MAC_SPECIFIC_LIBRARIES
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/macos/libwx_osx_cocoau_core-3.3.dylib
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/macos/libwx_osx_cocoau_html-3.3.dylib
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/macos/libwx_osx_cocoau_qa-3.3.dylib
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/macos/libwx_osx_cocoau_xrc-3.3.dylib
)

set(EXTERNAL_LIBRARIES
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/libsqlite3.a
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/libswift_net.a
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/libwx_baseu-3.3.dylib
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/libwx_baseu_net-3.3.dylib
    ${EXTERNAL_DIRECTORY_LOCATION}/lib/wx/libwx_baseu_xml-3.3.dylib
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

set(LINUX_FRAMEWORKS

)

# Collect all source files (scalable for many cpp files)
file(GLOB_RECURSE SOURCES "../src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Link wxWidgets libraries and macOS frameworks
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTERNAL_LIBRARIES})

if(APPLE)

    target_link_libraries(${PROJECT_NAME} PRIVATE ${MAC_SPECIFIC_LIBRARIES})

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

elseif(UNIX)

    foreach(FRAMEWORK ${LINUX_FRAMEWORKS})
        target_link_libraries(${PROJECT_NAME} PRIVATE "-framework ${FRAMEWORK}")
    endforeach()

    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _FILE_OFFSET_BITS=64
        wxDEBUG_LEVEL=0
    )    

endif()
