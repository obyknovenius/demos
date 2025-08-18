wayland_client_dep = dependency('wayland-client')

wayland_scanner = find_program('wayland-scanner')
xdg_shell_client_protocol_h = custom_target(
    'xdg-shell-client-protocol.h',
    input: '/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml',
    output: 'xdg-shell-client-protocol.h',
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'],
)
xdg_shell_client_protocol_c = custom_target(
    'xdg-shell-client-protocol.c',
    input: '/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml',
    output: 'xdg-shell-client-protocol.c',
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
)
xdg_shell_client_protocol_dep = declare_dependency(
    sources: [
        xdg_shell_client_protocol_h,
        xdg_shell_client_protocol_c
    ],
)

gui_deps = [
    core_dep,
    wayland_client_dep,
    xdg_shell_client_protocol_dep,
]

gui_sources = [
    'display.cpp',
    'wayland_display.cpp',
    'wayland_window.cpp',
]

gui_lib = static_library(
    'gui',
    sources: gui_sources,
    dependencies: gui_deps,
)

gui_dep = declare_dependency(
    include_directories: libs_inc,
    link_with: gui_lib,
)
