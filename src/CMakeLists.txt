add_executable(iss)

target_sources(iss
  PRIVATE
    main.c
    mem.c
    util.c
    iss.c
)

target_include_directories(iss
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
