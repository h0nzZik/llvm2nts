add_executable ( test_llvm2nts
	"test_main.cpp"
	"test_instructions.cpp"
)

include_directories(
	"../externals/Catch/include"
	".."
	${LLVM_INCLUDE_DIRS}
)

# Taken from http://llvm.org/docs/CMake.html
#
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core)

target_link_libraries ( test_llvm2nts llvm2nts ${llvm_libs})
