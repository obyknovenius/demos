project('client', 'cpp', 'c')

wayland_scanner = find_program('wayland-scanner')
xdg_shell_client_c = custom_target(
    'xdg-shell-client.c',
    input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
    output: ['xdg-shell-client.c'],
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
)
xdg_shell_client_h = custom_target(
    'xdg-shell-client.h',
    input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
    output: ['xdg-shell-client.h'],
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@']
)

wayland_client_dep = dependency('wayland-client')
wayland_egl_dep = dependency('wayland-egl')
egl_dep = dependency('egl')
gles2_dep = dependency('glesv2')
cairo_dep = dependency('cairo')

deps = [
    wayland_client_dep,
    wayland_egl_dep,
    egl_dep,
    gles2_dep,
    cairo_dep,
]

sources = [
    'client.cpp',
    'display.cpp',
    'label.cpp',
    'layer.cpp',
    'view.cpp',
    'window_button.cpp',
    'window_close_button.cpp',
    'window_maximize_button.cpp',
    'window.cpp',
    xdg_shell_client_c,
    xdg_shell_client_h,
]

executable(
    'client',
    sources: sources,
    dependencies: deps
)
