project('extensions', 'c',
        version : '1.4',
        default_options : ['warning_level=2'],
        meson_version: '>=1.1.0'
)

lpc_ext_lib = static_library('lpc_ext',
                             objects: ['../../../lpc-ext/src/lpc_ext.o'],
                             override_options: ['unity=off']
)

lpc_ext_dep = declare_dependency(
  include_directories : include_directories('../../../lpc-ext/src'),
  link_with : lpc_ext_lib
)

uuid_dep = dependency('uuid')
math_dep = meson.get_compiler('c').find_library('m', required : true)
sqlite_dep = dependency('sqlite3')
stats_dep = dependency('statisticslib')
glib_dep = dependency('glib-2.0', 
                      method: 'pkg-config',
                      required: true)

common_args = ['-fPIC', '-DPIC']

install_dir = meson.current_source_dir() / 'bin'

uuid_kfun = shared_module('uuid_kfun',
                          'kfun/uuid_kfun.c',
                          dependencies : [lpc_ext_dep, uuid_dep],
                          c_args : common_args,
                          name_prefix : '',
                          name_suffix : '1.0.0',
                          install : true,
                          install_dir : install_dir
)

math_kfun = shared_module('math_kfun',
                          'kfun/math_kfun.c',
                          dependencies : [lpc_ext_dep, math_dep],
                          c_args : common_args,
                          name_prefix : '',
                          name_suffix : '1.0.0',
                          install : true,
                          install_dir : install_dir
)

string_kfun = shared_module('string_kfun',
                            'kfun/string_kfun.c',
                            dependencies : [lpc_ext_dep],
                            c_args : common_args,
                            name_prefix : '',
                            name_suffix : '1.0.0',
                            install : true,
                            install_dir : install_dir
)

stats_kfun = shared_module('stats_kfun',
                           'kfun/statistics_kfun.c',
                           dependencies : [lpc_ext_dep, math_dep, stats_dep, glib_dep],
                           c_args : common_args,
                           name_prefix : '',
                           name_suffix : '1.0.0',
                           install : true,
                           install_dir : install_dir
)
