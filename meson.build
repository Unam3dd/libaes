project('aes', 'c',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'werror=true',
    'optimization=2',
    'debug=false',
    'strip=false',
    'b_pie=true',
    'b_staticpic=true',
    'c_std=c18',
    'buildtype=custom'
    ])


fs = import('fs')

#############################
#
#         INCLUDES
#
#############################

INC_DIR = ['inc']

#############################
#
#         SOURCES
#
#############################

SRCS_RC = ['src/rc/rc.c']
SRCS_PKCS = ['src/pkcs/pkcs.c']

SRCS = [SRCS_PKCS, SRCS_RC]
SRCS_FILES = files(SRCS)

#############################
#
#        GLOBAL FLAGS
#
#############################

CFLAGS = ['-D_FORTIFY_SOURCE=2', '-masm=intel']
LDFLAGS = []
TEST_CFLAGS = ['-g', '-fPIC']
#LDFLAGS = ['-nostdlib', '-nodefaultlibs']
PIE = true

STACK_PROTECTOR = '-fstack-protector'

#############################
#
#     OPTIONS
#
#############################

if (get_option('strip'))
  LDFLAGS += '-s'
endif

if (get_option('stack-protector') == 'strong')
  STACK_PROTECTOR = '-fstack-protector-strong'
elif (get_option('stack-protector') == 'all')
  STACK_PROTECTOR = '-fstack-protector-all'
elif (get_option('stack-protector') == 'no')
  STACK_PROTECTOR = '-fno-stack-protector'
endif

if (get_option('no-nx'))
  LDFLAGS += ['-z', 'execstack']
endif

CFLAGS += [STACK_PROTECTOR]

#############################
#
#         TESTS
#
#############################

TEST_SUBPROJECT = ['tests/pkcs', 'tests/rc']

#############################
#
#        COLORS
#
#############################

C_RST       = '\033[00m'
C_R         = '\033[31m'
C_G         = '\033[32m'
C_Y         = '\033[33m'
C_B         = '\033[34m'
C_P         = '\033[35m'
C_C         = '\033[36m'
C_W         = '\033[37m'

S_BOLD      = '\033[1m'
S_UNDERLINE = '\033[4m'
S_BLINK     = '\033[5m'
S_INVERSE   = '\033[7m'

#############################
#
#     PLATFORM
#
#############################

if (host_machine.system() != 'linux' or host_machine.cpu() != 'x86_64')
  error('\033[31m' + meson.project_name() + ' project must be compiled on Linux and use only on Linux !\033[00m')
endif

if get_option('c_std') == 'none'
  error('[\033[31m-\033[00m] C Standard not set !\033[00m')
endif

#############################
#
#     DISTRO_INFO
#
#############################

dist_info_name = 'undefined'
dist_info_pretty_name = 'undefined'
dist_info_version = 'undefined'
dist_info_version_id = 'undefined'
dist_info_codename = 'undefined'

if fs.is_file('/etc/os-release')
  os_release = fs.read('/etc/os-release').split('\n')

  foreach line: os_release
    if line.startswith('NAME')
      dist_info_name = line.split('=')[1].strip('"')
    endif

    if line.startswith('PRETTY_NAME')
      dist_info_pretty_name = line.split('=')[1].strip('"')
    endif

    if line.startswith('ID')
      dist_info_version = line.split('=')[1].strip('"')
    endif

    if line.startswith('VERSION_ID')
      dist_info_version_id = line.split('=')[1].strip('"')
    endif

    if line.startswith('VERSION_CODENAME')
      dist_info_codename = line.split('=')[1].strip('"')
    endif
  endforeach
endif

#############################
#
#     COMPILER OPTIONS
#
#############################

cc = meson.get_compiler('c')
ld = find_program('ld', required: true)

#############################
#
#       Header
#
#############################

HEADERS = ['aes.h', 'pkcs.h', 'buf.h']

foreach header: HEADERS
  if cc.has_header(header, include_directories: include_directories('inc'), required: true) == false
    error(C_R + header + ' not found !')
  endif
endforeach

#############################
#
#       Intrinsics
#
#############################


# Check if Intrinsics header of intel is present !

INTRIN_HEADER = ['immintrin.h', 'wmmintrin.h', 'nmmintrin.h', 'emmintrin.h', 'tmmintrin.h', 'x86gprintrin.h']

if get_option('no-aesni') == false
  foreach header: INTRIN_HEADER
    if cc.has_header(header) == false and get_option('no-aesni') == false
      error('You don\'t have the \'' + header + '\' header required to compile this AES library !\n' 
        + 'Maybe your CPU do not support SIMD or AES-NI extension ?')
    endif
  endforeach

  CFLAGS += ['-msse', '-msse2',  # Streaming SIMD Instruction (128 Bits XMM)
    '-msse3', '-msse4.1', 
    '-msse4.2']
  
  CFLAGS += ['-mavx', '-mavx2',   # AVX Instruction (256 Bits (XMM/YMM), 512 Bits (ZMM))
    '-mavx512bw', '-mavx512pf', 
    '-mavx512vl', '-mavx512f', 
    '-mavxvnni' ]
    #'-mavx512ifma']     # AVX Instruction (256 Bits, 512 Bits)
  
  CFLAGS += ['-maes']             # AES-NI Instruction
  
  CFLAGS += ['-mrdrnd']           # Read Random Instruction
  
  CFLAGS += ['-mvaes']            # Vectorize AES-NI Instruction
  
  CFLAGS += ['-mgfni']            # Galois Field Instruction
endif

#############################
#
#   Machine Build Information
#
#############################

summary({
  'Meson Version': S_BOLD + C_G + meson.version() + C_RST,
  'CPU': build_machine.cpu(),
  'Endian': S_BOLD + C_G + build_machine.endian() + C_RST,
  'System': build_machine.system(),
  'C Compiler': S_BOLD + cc.get_id() + ' ' + C_G + cc.version() + C_RST,
  'C Standard Used': S_BOLD + C_G + get_option('c_std') + C_RST,
  'Dynamic linker': ld,
  'Dynamic Linker version': S_BOLD + C_G + ld.version() + C_RST,
  }, section: 'Machine Build Information')

#############################
#
#     Project Information 
#
#############################

summary({
  'Name': S_BOLD + meson.project_name() + C_RST,
  'Version': S_BOLD + C_G + meson.project_version() + C_RST,
  'Root': meson.project_source_root(),
  'Optimization Level': S_BOLD + C_G + '-O' + get_option('optimization') + C_RST,
  'Build Type': S_BOLD + C_G + get_option('buildtype') + C_RST,
  'Debug mode': get_option('debug') == true ? S_BOLD + C_G + 'True' + C_RST : S_BOLD + C_R + 'False' + C_RST,
  'Prefix': S_BOLD + get_option('prefix') + C_RST,
  'Include Dir': get_option('prefix') + '/' + S_BOLD + C_G + get_option('includedir') + C_RST,
  'Library Dir': get_option('prefix') + '/' + S_BOLD + C_G + get_option('libdir') + C_RST,
  'Data Dir': get_option('prefix') + '/' + S_BOLD + C_G + get_option('datadir') + C_RST,
  'Bin Dir': get_option('prefix') + '/' + S_BOLD + C_G + get_option('bindir') + C_RST,
  }, section: 'Project Overview')

#############################
#
#    Distribution Info
#
#############################

summary({
  'Name': S_BOLD + C_G + dist_info_name + C_RST,
  'Pretty Name': S_BOLD + C_G + dist_info_pretty_name + C_RST,
  'Version': S_BOLD + C_G + dist_info_version + C_RST,
  'Version ID': S_BOLD + C_G + dist_info_version_id + C_RST,
  'Code Name': S_BOLD + C_G + dist_info_codename + C_RST
  }, section: 'Distribution Information')

#############################
#
#    Kernel Info
#
#############################

k = run_command('uname', '-s', check: true)
r = run_command('uname', '-r', check: true)
v = run_command('uname', '-v', check: true)

summary({
  'Kernel': S_BOLD + C_G + k.stdout().strip() + C_RST,
  'Release': S_BOLD + C_G +  r.stdout().strip() + C_RST,
  'Version': S_BOLD + C_G + v.stdout().strip() + C_RST,
  }, section: 'Kernel Information')

#############################
#
#        GLOBAL FLAGS
#
#############################

foreach args: CFLAGS
  add_project_arguments(args, language: 'c')
endforeach

foreach args: LDFLAGS
  add_project_link_arguments(args, language: 'c')
endforeach

#############################
#
#         BUILD 
#   STATIC/SHARED LIBRARY
#
#############################

lib = both_libraries(meson.project_name(),
                  SRCS_FILES,
                  install: true,
                  name_prefix: 'lib',
                  gnu_symbol_visibility: 'default',
                  install_dir: 'dist',
                  pic: PIE,
                  soversion: build_machine.cpu_family() + '_' + meson.project_version(),
                  include_directories: INC_DIR)

#############################
#
#     Library Summary
#
#############################

summary({
  'Dynamic Linked Library': S_BOLD + C_G + 'lib' + lib.get_shared_lib().name() + '.so' + C_RST,
  'Static Linked Library': S_BOLD + C_G +  'lib' + lib.get_static_lib().name() + '.a' + C_RST,
  'Architecture': S_BOLD + C_G + build_machine.cpu_family() + C_RST,
  'Version': S_BOLD + C_G + meson.project_version() + C_RST,
  'CFLAGS': S_BOLD + C_G + ' '.join(CFLAGS) + C_RST,
  'LDFLAGS': S_BOLD + C_G + ' '.join(LDFLAGS) + C_RST,
  'PIE (Position Independent Executable)': S_BOLD + C_G + PIE.to_string() + C_RST,
  }, section: 'Library Information')

#############################
#
#         TESTS
#
#############################

foreach TEST: TEST_SUBPROJECT
  subdir(TEST)
endforeach
subdir_done()
