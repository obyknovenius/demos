app_name = 'gles'
data_subdir = 'data'

pkgdatadir = get_option('prefix') / get_option('datadir') / app_name

conf = configuration_data()
conf.set_quoted('PKGDATADIR', pkgdatadir)

configure_file(output: 'config.h', configuration: conf)

install_data(data_subdir / 'container.jpg', install_dir: pkgdatadir)

executable(app_name, sources: 'main.cpp', dependencies: platform_dep)

meson.add_devenv(
    {
        'PKGDATADIR': meson.current_source_dir() / data_subdir,
    },
)
