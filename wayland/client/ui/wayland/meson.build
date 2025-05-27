wayland_scanner = find_program('wayland-scanner++')
xdg_shell_client = custom_target(
    'xdg-shell-client',
    input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
    output: ['xdg-shell-client.hpp', 'xdg-shell-client.cpp'],
    command: [wayland_scanner, '@INPUT@', '@OUTPUT@']
)

wayland_client_dep = dependency('wayland-client++')
wayland_egl_dep = dependency('wayland-egl++')

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
    xdg_shell_client[1]
]
