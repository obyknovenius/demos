egl_dep = dependency('egl')
gles2_dep = dependency('glesv2')
cairo_dep = dependency('cairo')

ui_deps = [
    egl_dep,
    gles2_dep,
    cairo_dep,
]

ui_sources = [
    'display_server.cpp',
    'layer.cpp',
    'text_view.cpp',
    'title_bar.cpp',
    'view.cpp',
    'window_button.cpp',
    'window_decoration_view.cpp',
    'window_close_button.cpp',
    'window_maximize_button.cpp',
    'window.cpp',
]

subdir('wayland')
subdir('gl')

libui = static_library('ui',
  sources: ui_sources,
  dependencies: ui_deps,
  include_directories: conf_inc,
)

libui_dep = declare_dependency(
    link_with: libui,
)
