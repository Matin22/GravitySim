cmake_minimum_required(VERSION 3.21)
project(GravitySim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Project Sources ---
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB APP_SOURCES "${SRC_DIR}/*.cpp")

# --- Dependencies ---

# 1. GLAD (Bundled with the template)
# We build GLAD from the source files included in the libs/ directory.
add_library(glad STATIC "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/lib/glad/include")

# 2. OpenGL (Found on the system)
find_package(OpenGL REQUIRED)

# 3. GLFW and GLM (Fetched automatically by CMake)
# This avoids needing users to install them system-wide.
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9 # A recent, stable version
)
# Prevent GLFW from building docs/tests/examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1 # A recent, stable version
)
FetchContent_MakeAvailable(glm)


# --- Executable Target ---
add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${SRC_DIR}"
    "${CMAKE_SOURCE_DIR}/lib/glad/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    OpenGL::GL
    glfw
    glm::glm
)

# --- Platform-Specific Settings ---
if (MSVC)
    # Silence common but safe warnings in Windows headers
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    # Set the working directory for the VS debugger to the project root
    # so it can find the 'res' folder easily.
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

if(APPLE)
    # Link necessary frameworks on macOS
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# --- Post-Build Step: Copy Shaders ---
# This command copies the 'res' folder next to the final executable.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    COMMENT "Copying resources to output directory"
)