set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(COMPILER_PREFIX armv8-rpi3-linux-gnueabihf-)

set(CMAKE_C_COMPILER    ${COMPILER_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${COMPILER_PREFIX}g++)
set(CMAKE_AR            ${COMPILER_PREFIX}ar)
set(CMAKE_RANLIB        ${COMPILER_PREFIX}ranlib)
# We use gcc as asm compiler only because of ninja build system.
# Cmake automatically generates rules.ninja file which has rules for c, c++ and asm identical, but
# options for gcc are not understood by the "as"... I could not find a way to force cmake to generate
# a different rule for asm, but it seems to be possible to compile asm with gcc, so let it be so.
# set(CMAKE_ASM_COMPILER  ${COMPILER_PREFIX}as)
set(CMAKE_ASM_COMPILER  ${COMPILER_PREFIX}gcc)
set(CMAKE_OBJCOPY       ${COMPILER_PREFIX}objcopy)
set(CMAKE_OBJDUMP       ${COMPILER_PREFIX}objdump)
set(CMAKE_SIZE          ${COMPILER_PREFIX}size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_EXE_LINKER_FLAGS "-static")

# to avoid compiling code (which may need special flags) we use this...
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
