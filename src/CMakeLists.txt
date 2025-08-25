add_executable(iss)
add_library(libiss)

set(SRCS
  main.c
  iss.c
  core.c
  main_mem.c
  rom.c
  halt.c
  text_buffer.c
  mem_map.c
  load_elf.c
  tick.c
  abstract_mem.c
)
target_sources(iss PRIVATE ${SRCS})
target_sources(libiss PRIVATE ${SRCS})

target_include_directories(iss
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(libiss
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(iss
  PUBLIC
    # -Wall -Wextra -Wpedantic -Werror
    -Wall -Werror
)
target_compile_options(libiss
  PUBLIC
    # -Wall -Wextra -Wpedantic -Werror
    -Wall -Werror
)
