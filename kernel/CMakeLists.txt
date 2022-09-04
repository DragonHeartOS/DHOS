set(SOURCES
    common/va_list.c
    kutil.c
    kernel.c
    drivers/comm/serial/serial.c
    drivers/video/framebuffer/fb.c
    interrupts/IDT.c
    common/log.c
    common/string.c)

add_executable (${KERNEL_TARGET} ${SOURCES})
target_include_directories (${KERNEL_TARGET} PUBLIC .)
set_property (TARGET ${KERNEL_TARGET} PROPERTY OUTPUT_NAME kernel)
set_property (TARGET ${KERNEL_TARGET} PROPERTY LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/kernel/linker.ld -nostdlib -zmax-page-size=0x1000 -static")
set (KERNEL_C_CXX_FLAGS -DKERNEL -ffreestanding -fno-stack-protector -fno-pic -mabi=sysv -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -MMD)

target_compile_options(${KERNEL_TARGET} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${C_COMPILER_FLAGS} ${KERNEL_C_CXX_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILER_FLAGS} ${KERNEL_C_CXX_FLAGS}>)

