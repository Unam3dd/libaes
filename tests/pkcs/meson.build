#############################
#
#       BUILD TEST
#
#############################

PKCS_SRCS = ['pkcs_test1', 'pkcs_null', 'pkcs_zero_buf_size', 'pkcs_zero_blk_size', 'pkcs_test2', 'pkcs_test3', 'pkcs_test4', 'pkcs_test5']

foreach PKCS_TEST: PKCS_SRCS
  PKCS_EXE = executable(PKCS_TEST + '.out',
                files(PKCS_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(PKCS_TEST, PKCS_EXE, verbose: false, suite: ['pkcs'])
endforeach

