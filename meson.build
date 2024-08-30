project('limos', 'c', version: '0.1', default_options: ['warning_level=3'])

inc = include_directories('include')

exe = executable('limos', 
    'src/main.c',
    'src/channel.c', 
    include_directories: inc,
    install: true)
