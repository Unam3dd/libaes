#############################
#
#       BUILD TEST
#
#############################

AES_CBC_TEST_SRCS = ['aes_128_cbc_one', 'aes_192_cbc_one', 'aes_256_cbc_one', 'aes_128_cbc_two', 'aes_cbc_one']

foreach AES_CBC_TEST: AES_CBC_TEST_SRCS
  AES_CBC_TEST_EXE = executable(AES_CBC_TEST + '.out',
                files(AES_CBC_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_CBC_TEST, AES_CBC_TEST_EXE, verbose: false, suite: ['aes-cbc-test-suite'])
endforeach

