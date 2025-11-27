#############################
#
#       BUILD TEST
#
#############################

AES_128_ECB_TEST_SRCS = ['aes_192_ecb_one', 'aes_256_ecb_one', 'aes_256_ecb_two', 'aes_128_ecb_one', 'aes_128_ecb_two', 'aes_ecb_one']

foreach AES_128_ECB_TEST: AES_128_ECB_TEST_SRCS
  AES_128_ECB_TEST_EXE = executable(AES_128_ECB_TEST + '.out',
                files(AES_128_ECB_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_128_ECB_TEST, AES_128_ECB_TEST_EXE, verbose: false, suite: ['aes-ecb-test-suite'])
endforeach

