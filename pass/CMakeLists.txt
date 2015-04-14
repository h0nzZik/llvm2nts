add_library(pass_llvm2nts MODULE
	"Pass.cpp"
)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories ("..")
target_link_libraries(pass_llvm2nts llvm2nts)



