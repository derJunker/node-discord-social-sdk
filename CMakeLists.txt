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

# Define source files
file(GLOB SOURCE_FILES "src/native/*.cpp" "src/native/*.h")

# Create the addon
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Link libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Define NAPI_VERSION
add_definitions(-DNAPI_VERSION=8)
