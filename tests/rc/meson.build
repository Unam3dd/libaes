#############################
#
#       BUILD TEST
#
#############################

PKCS_SRCS = ['rc_test1']

foreach PKCS_TEST: PKCS_SRCS
  PKCS_EXE = executable(PKCS_TEST + '.out',
                files(PKCS_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(PKCS_TEST, PKCS_EXE, verbose: false, suite: ['rc'])
endforeach

