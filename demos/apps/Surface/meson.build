egl_dep = dependency('egl')
gles2_dep = dependency('glesv2')
wayland_egl_dep = dependency('wayland-egl')

executable(
    'surface',
    sources: 'main.cpp',
    dependencies: [
        egl_dep,
        gles2_dep,
        gui_dep,
        wayland_egl_dep,
    ]
)
