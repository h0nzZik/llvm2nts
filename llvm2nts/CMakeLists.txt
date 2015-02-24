add_library(llvm2nts SHARED
	"llvm2nts.cpp"
	"llvmFunction2nts.cpp"
	"FunctionMapping.cpp"
	"instructions/InstAdd.cpp"
	"instructions/InstLoadStore.cpp"
	"instructions/InstBr.cpp"
)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories ( ".." )

target_link_libraries ( llvm2nts ntslib )

