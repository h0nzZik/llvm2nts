add_executable ( test_llvm2nts
	"test_main.cpp"
	"test_instructions.cpp"
)
include_directories( ${LLVM_INCLUDE_DIRS} )
include_directories(
	"../externals/Catch/include"
	".."
)

target_link_libraries ( test_llvm2nts llvm2nts )
