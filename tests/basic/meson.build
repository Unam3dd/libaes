#############################
#
#       BUILD TEST
#
#############################

BASIC_TEST1_SRC = ['./test1.c']
basic_test1_exe = executable('test1',
                files(BASIC_TEST1_SRC),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)

test('test1', basic_test1_exe, verbose: true, suite: ['basic'])
