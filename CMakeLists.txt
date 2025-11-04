cmake_minimum_required(VERSION 3.23)

project(solarpass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

add_executable(solarpass)
target_sources(solarpass
	PRIVATE
		main.cpp
)

target_include_directories(solarpass PRIVATE
    ${SODIUM_INCLUDE_DIRS}
)

target_link_libraries(solarpass PRIVATE ${SODIUM_LIBRARIES})
add_subdirectory(clip)

if(APPLE)
    target_link_libraries(solarpass PRIVATE ${SODIUM_LIBRARIES} clip)
elseif(UNIX)
    target_link_libraries(solarpass PRIVATE ${SODIUM_LIBRARIES} clip X11)
elseif(WIN32)
    target_link_libraries(solarpass PRIVATE ${SODIUM_LIBRARIES} clip)
endif()

if (MSVC)
    target_compile_options(solarpass PRIVATE /W4)
else()
    target_compile_options(solarpass PRIVATE -Wall -Wextra -pedantic)
endif()
