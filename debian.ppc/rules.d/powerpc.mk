human_arch	= PowerPC (32 bit userspace)
build_arch	= powerpc
header_arch	= $(build_arch)
defconfig	= pmac32_defconfig
flavours	= powerpc-smp powerpc64-smp
build_image	= vmlinux
kernel_file	= $(build_image)
install_file	= $(build_image)

loader		= yaboot

custom_flavours	=

no_dumpfile = true
skipdbg		= true
skipabi		= true
skipmodule	= true
do_doc_package          = false
do_source_package       = false
do_common_headers_indep = false
do_libc_dev_package     = false

family=ubuntu
