human_arch	= PowerPC (32 bit userspace)
build_arch	= powerpc
header_arch	= $(build_arch)
defconfig	= pmac32_defconfig
flavours	= powerpc-smp powerpc64-smp powerpc-e500 powerpc-e500mc
build_image	= vmlinux
kernel_file	= $(build_image)
install_file	= $(build_image)

# These flavours differ
build_image_powerpc-e500mc	= uImage
kernel_file_powerpc-e500mc	= arch/powerpc/boot/uImage

build_image_powerpc-e500	= uImage
kernel_file_powerpc-e500	= arch/powerpc/boot/uImage

loader		= yaboot

custom_flavours	=

no_dumpfile		= true
skipdbg			= true
skipabi			= true
skipmodule		= true
do_doc_package          = false
do_source_package       = false
do_common_headers_indep = true
do_libc_dev_package     = false
do_tools		= false

family			= ubuntu
