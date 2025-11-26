priv_incdir = include_directories('.')
pub_incdir = include_directories('../include')

lib_src = files(
  'abstract_mem.c',
  'core.c',
  'halt.c',
  'iss.c',
  'load_elf.c',
  'main_mem.c',
  'mem_map.c',
  'rom.c',
  'text_buffer.c',
  'tick.c'
)

common_c_args  = ['-Wall', '-Werror', '-O2']

lib = library('iss', sources: lib_src, include_directories: [priv_incdir, pub_incdir], c_args: common_c_args)
libiss_dep = declare_dependency(include_directories: pub_incdir, link_with: lib)

main = executable('main', 'main.c', dependencies: libiss_dep, c_args: common_c_args)
