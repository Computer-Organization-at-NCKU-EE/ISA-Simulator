# CMake version requirements
cmake_minimum_required(VERSION 3.21...3.28)
if(${CMAKE_VERSION} VERSION_LESS 3.28)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION} ${CMAKE_MINOR_VERSION})
endif()

# project information
project(RISCV-ISS VERSION 0.1.0
  DESCRIPTION "A RISC-V ISA Simulator"
  LANGUAGES C)

# set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Build rules for target main
add_executable(
  # target name
  main
  # sources
  src/main.c
  src/iss.c
  src/mem.c
  src/util.c
)
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)
