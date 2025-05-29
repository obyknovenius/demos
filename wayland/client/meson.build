project('client', 'cpp', 'c')

sources = [
    'client.cpp',
]

conf_inc = include_directories('.')
ui_inc = include_directories('ui')

subdir('ui')

executable(
    'client',
    install: true,
    sources: sources,
    dependencies: [libui_dep],
    include_directories: [conf_inc, ui_inc],
    link_with: libui,
)
