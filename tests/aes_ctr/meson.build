#############################
#
#       BUILD TEST
#
#############################

AES_CTR_TEST_SRCS = ['aes_192_ctr_one', 'aes_256_ctr_one', 'aes_128_ctr_one', 'aes_128_ctr_two', 'aes_128_ctr_random', 'aes_128_ctr_three', 'aes_128_ctr_128_nonce']

foreach AES_CTR_TEST: AES_CTR_TEST_SRCS
  AES_CTR_TEST_EXE = executable(AES_CTR_TEST + '.out',
                files(AES_CTR_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_CTR_TEST, AES_CTR_TEST_EXE, verbose: false, suite: ['aes-ctr-test-suite'])
endforeach

