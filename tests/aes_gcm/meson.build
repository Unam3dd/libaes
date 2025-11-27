#############################
#
#       BUILD TEST
#
#############################

AES_GCM_TEST_SRCS = [
  'aes_gcm_nist_validation',
  'aes_128_gcm_test1',
  'aes_128_gcm_test2',
  'aes_128_gcm_test3_aad',
  'aes_128_gcm_test4_multiblock',
  'aes_128_gcm_test5_partial_block',
  'aes_192_gcm_test',
  'aes_256_gcm_test',
  'aes_128_gcm_128_nonce'
]

foreach AES_GCM_TEST: AES_GCM_TEST_SRCS
  AES_GCM_TEST_EXE = executable(AES_GCM_TEST + '.out',
                files(AES_GCM_TEST + '.c'),
                c_args: ['-g', '-fPIC'],
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_GCM_TEST, AES_GCM_TEST_EXE, verbose: false, suite: ['aes-gcm-test-suite'])
endforeach

