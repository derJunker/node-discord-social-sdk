cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project(discord_social_sdk)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include N-API wrappers
execute_process(
  COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Remove quotes if present
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

include_directories(${CMAKE_JS_INC})
include_directories(${NODE_ADDON_API_DIR})

# Define some handy Social SDK variables
set(DISCORD_SDK_ROOT "${CMAKE_SOURCE_DIR}/lib/discord_social_sdk")
set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_ROOT}/lib/release")
set(DISCORD_SDK_BIN_DIR "${DISCORD_SDK_ROOT}/bin/release")
set(DISCORD_SDK_INCLUDE_DIR "${DISCORD_SDK_ROOT}/include")
set(DISCORD_SDK_HEADER "${DISCORD_SDK_INCLUDE_DIR}/discordpp.h")

# Include for Social SDK headers (if SDK is present)
if(EXISTS "${DISCORD_SDK_HEADER}")
    include_directories(${DISCORD_SDK_INCLUDE_DIR})
    add_definitions(-DUSE_DISCORD_SDK)
    message(STATUS "Discord Social SDK found at ${DISCORD_SDK_ROOT}")
else()
    message(STATUS "Discord Social SDK not found. Building without SDK support.")
    message(STATUS "To enable Discord integration, place the SDK in ${DISCORD_SDK_ROOT}")
endif()

# Define source files
file(GLOB SOURCE_FILES "src/native/*.cpp" "src/native/*.h")

# Create the addon
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Link libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Platform-specific Social SDK library paths (if SDK is present)
if(EXISTS "${DISCORD_SDK_HEADER}")
    if(WIN32)
        set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/discord_partner_sdk.lib")
        set(DISCORD_SHARED_LIB "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll")
    elseif(APPLE)
        set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
        set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
    else() # Linux
        set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
        set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
    endif()

    # Link the Social SDK library if it exists
    if(EXISTS "${DISCORD_LIB_PATH}")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${DISCORD_LIB_PATH})
        message(STATUS "Linking Discord Social SDK: ${DISCORD_LIB_PATH}")
    endif()

    # Set the runtime search path (RPATH) for Linux and macOS
    if(UNIX)
        # Use RPATH when building
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        # Set the RPATH to use the lib directory relative to the executable
        set(CMAKE_INSTALL_RPATH "$ORIGIN")
        if(APPLE)
            set(CMAKE_INSTALL_RPATH "@executable_path")
        endif()
    endif()

    # Copy Social SDK shared library to output directory, so it's available at runtime.
    if(EXISTS "${DISCORD_SHARED_LIB}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DISCORD_SHARED_LIB}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
        message(STATUS "Will copy Discord SDK library to output: ${DISCORD_SHARED_LIB}")
    endif()
endif()

# Define NAPI_VERSION
add_definitions(-DNAPI_VERSION=8)
