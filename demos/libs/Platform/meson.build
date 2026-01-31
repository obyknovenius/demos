platform_deps = [
    core_dep,
    gfx_dep,
]

platform_sources = [
    'Display.cpp',
    'Window.cpp',
]

subdir('Wayland')

platform_lib = static_library(
    'Platform',
    platform_sources,
    dependencies: platform_deps,
)

platform_dep = declare_dependency(
    link_with: platform_lib,
    dependencies: platform_deps,
)
