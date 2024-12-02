#############################
#
#       BUILD TEST
#
#############################

AES_OFB_TEST_SRCS = ['aes_192_ofb_one', 'aes_256_ofb_one', 'aes_128_ofb_one', 'aes_128_ofb_two', 'aes_128_ofb_random']

foreach AES_OFB_TEST: AES_OFB_TEST_SRCS
  AES_OFB_TEST_EXE = executable(AES_OFB_TEST + '.out',
                files(AES_OFB_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_OFB_TEST, AES_OFB_TEST_EXE, verbose: false, suite: ['aes-ofb-test-suite'])
endforeach

