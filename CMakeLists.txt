cmake_minimum_required(VERSION 3.5)
project(AncientGrammar)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES )
list(APPEND SOURCE_FILES
src/verb.cpp
src/utf8proc/utf8proc.c
src/utils.cpp)

set(PUBLIC_HEADER_FILES )
list(APPEND PUBLIC_HEADER_FILES
include/verb.h
include/utils.h)

set(OTHER_HEADER_FILES )
list(APPEND OTHER_HEADER_FILES
include/utf8proc.h
include/json.h)

add_library(AncientGrammar ${SOURCE_FILES} ${PUBLIC_HEADER_FILES} ${OTHER_HEADER_FILES})

link_directories(${CMAKE_BINARY_DIR})

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()


if(IOS)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fembed-bitcode")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fembed-bitcode")

    set(CMAKE_OSX_SYSROOT iphoneos)

    # Chooses the correct arch
    set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_64_BIT)")

    # seamless toggle between device and simulator - one is ARM, the other is intel
    set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")

    set_target_properties(AncientGrammar PROPERTIES
    FRAMEWORK TRUE
    MACOSX_FRAMEWORK_IDENTIFIER com.mrattoattoatto.AncientGrammarLib
    PUBLIC_HEADER "${PUBLIC_HEADER_FILES}"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
endif()


if(NOT IOS)
    add_executable(Test src/test.cpp)
    target_link_libraries(Test AncientGrammar)
    add_dependencies(Test AncientGrammar)
endif()
