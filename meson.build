project('do', 'c',
  version: '0.1',
  license: 'MPL-2.0',
  default_options: ['warning_level=3', 'bindir=/usr/local/sbin'])

# define source files
src = files('src/main.c')

# define project metadata
url = 'https://github.com/mekb-turtle/do'
name = meson.project_name()
version = meson.project_version()

add_project_arguments(
  f'-DPROJECT_NAME="@name@"',
  f'-DPROJECT_VERSION="@version@"',
  f'-DPROJECT_URL="@url@"',
  language : 'c')

exe = executable('do', sources: src, install: true, install_mode: ['rwsr-xr-x', 'root', 'root'])

