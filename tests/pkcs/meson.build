#############################
#
#       BUILD TEST
#
#############################

PKCS_TEST_SRCS = ['pkcs_test_null', 'pkcs_test_invalid_size', 'pkcs_test_one', 'pkcs_test_two', 'pkcs_test_three', 
  'pkcs_test_four', 'pkcs_test_five', 'pkcs_test_six', 'pkcs_test_seven', 'pkcs_test_unpad_null', 'pkcs_test_unpad_invalid_size']

foreach PKCS_TEST: PKCS_TEST_SRCS
  PKCS_TEST_EXE = executable(PKCS_TEST + '.out',
                files(PKCS_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(PKCS_TEST, PKCS_TEST_EXE, verbose: false, suite: ['pkcs-test-suite'])
endforeach

