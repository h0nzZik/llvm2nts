add_library(llvm2nts MODULE
	Pass.cpp
	llvm2nts.cpp
	VariableManager.cpp
)

add_subdirectory(nts)

target_link_libraries(llvm2nts ntslib)
