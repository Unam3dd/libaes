#############################
#
#       BUILD TEST
#
#############################

RAND_TEST_SRCS = ['urandom_1', 'rdrnd_1', 'rdrnd_2']

foreach RAND_TEST: RAND_TEST_SRCS
  RAND_TEST_EXE = executable(RAND_TEST + '.out',
                files(RAND_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(RAND_TEST, RAND_TEST_EXE, verbose: false, suite: ['random'])
endforeach

