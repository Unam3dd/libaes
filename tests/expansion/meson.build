#############################
#
#       BUILD TEST
#
#############################

AES_EXPANSION_TEST_SRCS = ['aes_expansion_128']

foreach AES_EXPANSION_TEST: AES_EXPANSION_TEST_SRCS
  AES_EXPANSION_TEST_EXE = executable(AES_EXPANSION_TEST + '.out',
                files(AES_EXPANSION_TEST + '.c'),
                c_args: TEST_CFLAGS,
                link_args: TEST_LDFLAGS,
                link_with: lib,
                include_directories: ['../../inc/', '.'],
                install: false,
                pie: true)
  test(AES_EXPANSION_TEST, AES_EXPANSION_TEST_EXE, verbose: false, suite: ['expansion'])
endforeach

