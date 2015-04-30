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

include_directories (
	${LLVM_INCLUDE_DIRS}
	"../externals"
	)

# Taken from http://llvm.org/docs/CMake.html
#
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core irreader)

target_link_libraries ( llvm2nts NTS_cpp ${llvm_libs} )

