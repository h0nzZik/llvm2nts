set ( lib_name "llvm2nts" )

add_library(llvm2nts SHARED
	"llvm2nts.cpp"
	"llvmFunction2nts.cpp"
	"FunctionMapping.cpp"
	"ModuleMapping.cpp"
	"types.cpp"
	"util.cpp"
	"instructions/InstAdd.cpp"
	"instructions/InstLoadStore.cpp"
	"instructions/InstAlloca.cpp"
	#	"instructions/InstBr.cpp"
	"instructions/InstIcmp.cpp"
	"instructions/InstCall.cpp"
)

include_directories ( ${LLVM_INCLUDE_DIRS} )

# Taken from http://llvm.org/docs/CMake.html
#
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core irreader)

target_link_libraries ( llvm2nts NTS_cpp ${llvm_libs} )


set(config_install_dir "lib/cmake/${lib_name}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(lib_version_config "${generated_dir}/${lib_name}-configVersion.cmake")
set(lib_config "${generated_dir}/${lib_name}-config.cmake")
set(targets_export_name "${lib_name}-targets")

include ( CMakePackageConfigHelpers )

write_basic_package_version_file( "${lib_version_config}"
	VERSION "0.1"
	COMPATIBILITY AnyNewerVersion
)

configure_file ( "Config.cmake.in" "${lib_config}" @ONLY )


install (
	TARGETS llvm2nts
	EXPORT "${targets_export_name}"
	LIBRARY DESTINATION "lib"
	ARCHIVE DESTINATION "lib"
	RUNTIME DESTINATION "bin"
	INCLUDES DESTINATION "${include_install_dir}"
)

# Install all header files
install(
	FILES
		"llvm2nts.hpp"

	DESTINATION
		"${include_install_dir}/llvm2nts"
)

# Configuration files
install(
	FILES
		"${lib_config}"
		"${lib_version_config}"

	DESTINATION
		"${config_install_dir}"
)

install(
	EXPORT
		"${targets_export_name}"
#	NAMESPACE
#		"${namespace}"
	DESTINATION
		"${config_install_dir}"
)

