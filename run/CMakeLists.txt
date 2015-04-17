add_executable ( run
	"main.cpp"
)
include_directories( ${LLVM_INCLUDE_DIRS} )
include_directories(
	".."
)

# Taken from http://llvm.org/docs/CMake.html
#
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core irreader)

target_link_libraries ( run llvm2nts ${llvm_libs})
