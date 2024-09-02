project('limos', 'c', version: '0.1', default_options: ['warning_level=3'])

inc = include_directories('include')
src = ['src/channel.c', 'src/worker.c']

dependencies = [
    dependency('threads')
]

exe = executable('limos', 
    sources: src + ['src/main.c'],
    include_directories: inc,
    dependencies: dependencies,
    install: true)

test('test channel', executable(
    'test_channel', 
    sources: src + ['tests/test_channel.c'],
    dependencies: dependencies,
    include_directories: inc))

test('test worker', executable(
    'test_worker', 
    sources: src + ['tests/test_worker.c'],
    dependencies: dependencies,
    include_directories: inc))
