#############################
#
#       BUILD TEST
#
#############################

AES_CFB_TEST_SRCS = ['aes_192_cfb_one', 'aes_256_cfb_one', 'aes_256_cfb_two', 'aes_128_cfb_one', 'aes_128_cfb_two', 'aes_128_cfb_random']

foreach AES_CFB_TEST: AES_CFB_TEST_SRCS
  AES_CFB_TEST_EXE = executable(AES_CFB_TEST + '.out',
                files(AES_CFB_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_CFB_TEST, AES_CFB_TEST_EXE, verbose: false, suite: ['aes-cfb-test-suite'])
endforeach

