gui_deps = [
    core_dep,
    gfx_dep,
]

gui_sources = [
    'Application.cpp',
    'View.cpp',
    'WindowCloseButton.cpp',
    'WindowDecorationView.cpp',
    'WindowTitleBar.cpp',
    'Window.cpp',
]

subdir('Wayland')

gui_lib = static_library(
    'GUI',
    sources: gui_sources,
    dependencies: gui_deps,
)

gui_dep = declare_dependency(
    include_directories: libs_inc,
    link_with: gui_lib,
)
