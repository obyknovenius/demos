cairo_dep = dependency('cairo')

gfx_deps = [
    core_dep,
    cairo_dep,
]

gfx_sources = [
    'Color.cpp',
]

subdir('Cairo')
subdir('Wayland')

gfx_lib = static_library(
    'Gfx',
    sources: gfx_sources,
    dependencies: gfx_deps,
)

gfx_dep = declare_dependency(
    include_directories: libs_inc,
    link_with: gfx_lib,
    dependencies: gfx_deps,
)
