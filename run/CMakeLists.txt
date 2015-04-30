add_executable ( run
	"main.cpp"
)
include_directories( ${LLVM_INCLUDE_DIRS} )
include_directories(
	".."
)

target_link_libraries ( run llvm2nts )
