project('client', 'c')

wayland_scanner = find_program('wayland-scanner')
xdg_shell_protocol_c = custom_target('xdg-shell-protocol.c',
                                     input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
                                     output: ['xdg-shell-protocol.c'],
                                     command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@'])
xdg_shell_protocol_h = custom_target('xdg-shell-protocol.h',
                                     input: ['/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml'],
                                     output: ['xdg-shell-protocol.h'],
                                     command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'])

wayland_client = dependency('wayland-client')

executable('client', ['client.c', xdg_shell_protocol_c, xdg_shell_protocol_h], dependencies: wayland_client)