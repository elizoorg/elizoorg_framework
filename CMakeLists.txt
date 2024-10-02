# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.16)
# Enable Hot Reload for MSVC compilers if supported.



project (elizoorg)


set(Example_VERSION_MAJOR 2)
set(Example_VERSION_MINOR 3)
set(Example_VERSION_PATCH 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 20)

# Include sub-projects.
add_subdirectory("DirectXCourseProject")
add_subdirectory("GameApplication")


find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameApplication)




set(VCPKG_CONFIGURE_MAKE_OPTIONS VCPKG_CMAKE_CONFIGURE_OPTIONS_DEBUG)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${Example_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Example_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Example_VERSION_PATCH})
include(CPack)
