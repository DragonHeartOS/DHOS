cmake_minimum_required (VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_LINKER "${CMAKE_SOURCE_DIR}/toolchain/build/bin/x86_64-elf-ld")
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_C_COMPILER "/usr/bin/tcc" CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/tcc" CACHE STRING "C++ compiler" FORCE)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(dhos VERSION 0.1 LANGUAGES C CXX ASM_NASM)

set(WARNING_FLAGS -Wall -Wextra -Wconversion -Wsign-conversion -Wno-varargs)
set(COMMON_COMPILER_FLAGS ${WARNING_FLAGS})
set(C_COMPILER_FLAGS ${COMMON_COMPILER_FLAGS})
set(CXX_COMPILER_FLAGS ${COMMON_COMPILER_FLAGS} -fno-exceptions -fno-rtti)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

set(KERNEL_TARGET kernel)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(kernel)

add_custom_target(image)
add_custom_command(
  TARGET   image
  COMMAND  "../util/build_disk_image.sh"
  USES_TERMINAL
)

add_custom_target(cdrom)
add_custom_command(
  TARGET   cdrom
  COMMAND  "../util/build_cd_image.sh"
)

add_custom_target(run)
add_custom_command(
  TARGET   run
  COMMAND  "../util/run.sh"
  USES_TERMINAL
)

