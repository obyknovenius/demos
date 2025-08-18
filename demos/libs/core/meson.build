core_sources = [
    'event_loop.cpp',
]

core_lib = static_library(
    'core',
    sources: core_sources,
)

core_dep = declare_dependency(
    include_directories: libs_inc,
    link_with: core_lib,
)
