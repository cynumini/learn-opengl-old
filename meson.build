project(
    'learn-opengl',
    'c',
    default_options: ['warning_level=everything', 'c_std=c23'],
)

sources = [
    'src/main.c',
    'src/error_callback.c',
]

glad_inc = include_directories('glad/include')
glad_lib = static_library('glad', 'glad/src/gl.c', include_directories: glad_inc)
glad_dep = declare_dependency(link_with: glad_lib, include_directories: glad_inc)

dependencies = [dependency('glfw3'), glad_dep]

exe = executable(
    'learn-opengl',
    sources,
    dependencies: dependencies,
    install: true,
)

test('basic', exe)

run_target('run', command: exe)
