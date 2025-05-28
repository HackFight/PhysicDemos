cmake_minimum_required(VERSION "4.0.2")

set(CMAKE_CXX_STANDARD 17)

#Set this to true for shipping!
set(PRODUCTION_BUILD FALSE CACHE BOOL "Make this a production build" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

if(PRODUCTION_BUILD)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimisation
else()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

if(MSVC)
add_compile_options(/arch:AVX2) #Optimises some stuff... idk
endif()


#Main project
project(main)

# GLAD
add_library(glad STATIC thirdParty/glad/src/glad.c)
target_include_directories(glad PUBLIC thirdParty/glad/include)

# GLFW configuration
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build GLFW as static lib" FORCE)

add_subdirectory("thirdParty/glfw")
add_subdirectory("thirdParty/glm")

# We create MY_SOURCES, a list of all the source files of the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(main "${MY_SOURCES}")

# We make the RESOURCES_PATH constant that sends to the resources folder
if(PRODUCTION_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") #Relative path
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)
else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") #Absolute path... ONLY DURING DEVELOPEMENT!
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
endif()

if(MSVC)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS) #Remove useless warnings
endif()

target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(main PRIVATE glfw glad glm)

# Platform-specific dependencies
find_package(OpenGL REQUIRED)
target_link_libraries(main PRIVATE OpenGL::GL)

if(UNIX AND NOT APPLE)
    target_link_libraries(main PRIVATE dl pthread X11 Xrandr Xi Xxf86vm Xcursor)
endif()