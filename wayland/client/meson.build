project('client', 'c')

wayland_scanner = find_program('wayland-scanner')
xdg_shell_protocol_c = custom_target(
    'xdg-shell-protocol.c',
    input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
    output: ['xdg-shell-protocol.c'],
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
)
xdg_shell_protocol_h = custom_target(
    'xdg-shell-protocol.h',
    input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
    output: ['xdg-shell-protocol.h'],
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@']
)

wayland_client_dep = dependency('wayland-client')
wayland_egl_dep = dependency('wayland-egl')
egl_dep = dependency('egl')
gles2_dep = dependency('glesv2')

client_deps = [
    wayland_client_dep,
    wayland_egl_dep,
    egl_dep,
    gles2_dep,
]

executable(
    'client',
    sources: ['client.c', xdg_shell_protocol_c, xdg_shell_protocol_h],
    dependencies: client_deps
)