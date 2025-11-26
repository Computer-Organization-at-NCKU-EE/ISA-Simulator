priv_incdir = include_directories('.', '../include')
pub_incdir = include_directories('../include')

lib_src = [
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
]

lib = library('iss', sources: lib_src, include_directories: [priv_incdir, pub_incdir])
main = executable('main', 'main.c', include_directories: pub_incdir, link_with: lib)
