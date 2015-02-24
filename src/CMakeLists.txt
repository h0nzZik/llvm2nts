add_library(llvm2nts MODULE
	"Pass.cpp"
	"llvm2nts.cpp"
	"llvmFunction2nts.cpp"
	"FunctionMapping.cpp"
	"instructions/InstAdd.cpp"
	"instructions/InstLoadStore.cpp"
	"instructions/InstBr.cpp"
)

add_subdirectory(nts)
add_subdirectory(test_nts)

target_link_libraries(llvm2nts ntslib)
