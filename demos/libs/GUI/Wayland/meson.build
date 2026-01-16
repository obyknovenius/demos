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

tablet_v2_client_protocol_h = custom_target(
    'tablet-v2-client-protocol.h',
    input: '/usr/share/wayland-protocols/stable/tablet/tablet-v2.xml',
    output: 'tablet-v2-client-protocol.h',
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'],
)
tablet_v2_client_protocol_c = custom_target(
    'tablet-v2-client-protocol.c',
    input: '/usr/share/wayland-protocols/stable/tablet/tablet-v2.xml',
    output: 'tablet-v2-client-protocol.c',
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
)
tablet_v2_client_protocol_dep = declare_dependency(
    sources: [
        tablet_v2_client_protocol_h,
        tablet_v2_client_protocol_c
    ],
)


cursor_shape_v1_client_protocol_h = custom_target(
    'cursor-shape-v1-client-protocol.h',
    input: '/usr/share/wayland-protocols/staging/cursor-shape/cursor-shape-v1.xml',
    output: 'cursor-shape-v1-client-protocol.h',
    command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'],
)
cursor_shape_v1_client_protocol_c = custom_target(
    'cursor-shape-v1-client-protocol.c',
    input: '/usr/share/wayland-protocols/staging/cursor-shape/cursor-shape-v1.xml',
    output: 'cursor-shape-v1-client-protocol.c',
    command: [wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@']
)
cursor_shape_v1_client_protocol_dep = declare_dependency(
    sources: [
        cursor_shape_v1_client_protocol_h,
        cursor_shape_v1_client_protocol_c
    ],
    dependencies: [
        tablet_v2_client_protocol_dep
    ]
)

gui_wayland_deps = [
    wayland_client_dep,
    xdg_shell_client_protocol_dep,
    cursor_shape_v1_client_protocol_dep,
]

gui_deps += gui_wayland_deps

gui_wayland_sources = [
    'Display.cpp',
    'Pointer.cpp',
    'Seat.cpp',
    'Surface.cpp',
    'Toplevel.cpp',
    'Window.cpp',
]

gui_sources += files(gui_wayland_sources)
