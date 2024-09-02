project('limos', 'c', version: '0.1', default_options: ['warning_level=3'])

inc = include_directories('include')
src = ['src/channel.c']

exe = executable('limos', 
    sources: src + ['src/main.c'],
    include_directories: inc,
    install: true)

test('test channel', executable(
    'test_channel', 
    sources: src + ['tests/test_channel.c'],
    include_directories: inc))
