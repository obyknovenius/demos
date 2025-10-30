core_sources = [
    'EventLoop.cpp',
]

core_lib = static_library(
    'Core',
    sources: core_sources,
)

core_dep = declare_dependency(
    include_directories: libs_inc,
    link_with: core_lib,
)
