add_executable(test_ntslib
	"main.cpp"
	"test_Printable.cpp"
)

include_directories(
	"../externals/Catch/include"
	".."
	)

target_link_libraries(test_ntslib ntslib)
