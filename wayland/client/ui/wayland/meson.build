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

ui_deps += [
    wayland_client_dep,
    wayland_egl_dep,
]

ui_wayland_sources = [
    'server.cpp',
    'window.cpp',
]

ui_sources += [
    files(ui_wayland_sources),
    xdg_shell_client_c,
    xdg_shell_client_h,
]
