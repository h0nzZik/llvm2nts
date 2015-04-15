#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <catch.hpp>

#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>

#include <libNTS/nts.hpp>

#include <llvm2nts/llvm2nts.hpp>



using namespace nts;
using namespace llvm;

static void do_file_compare_test ( const char *llfile,
		const char *ntsfile = NULL,
		bool print = false)
{
	llvm::SMDiagnostic diag;
	LLVMContext        ctx;

	std::unique_ptr<llvm::Module> m = llvm::parseIRFile ( llfile, diag, ctx );

	if ( !m )
	{
		std::cerr << "Error on line " << diag.getLineNo() << ":\n";
		std::cerr << diag.getMessage().str();
		throw std::runtime_error ( "Can not parse given llvm IR file" );
	}

	std::stringstream gen_ss;
	try {
		auto nts = llvm_to_nts ( *m );
		gen_ss << *nts;
		if ( print )
			std::cout << gen_ss.str() << std::endl;
	} catch ( std::exception &e ) {
		auto s = std::string ( "An exception was thrown while processing:\n" ) +  e.what();
		FAIL (s);
	}

	if ( !ntsfile )
		return;

	// Compare generated file with reference.
	// (It is not very nice and should be replaced by something
	// more sophisticated.)

	std::ifstream in ( ntsfile );
	std::string in_line;

	std::stringstream err;
	int ln = 1;
	while (std::getline ( in, in_line ))
	{
		std::string nts_line;
		if ( !std::getline ( gen_ss, nts_line ) )
		{
			err << "Files have different length";
			FAIL ( err.str() );
		}

		if ( nts_line != in_line )
		{
			err << "File missmatch at line " << ln << "\n";
			err << "\tOriginal line: '" << in_line << "'\n";
			err << "\tGenerated line: '" << nts_line << "'\n";
			FAIL ( err.str() );
		}

		ln++;
	}

}

TEST_CASE ( "VoidFunction", "Simple void function")
{
	do_file_compare_test (
			"test_cases/00_empty.ll",
			"test_cases/00_empty.nts"
	);
}

TEST_CASE ( "LoadStoreAlloca", "Tests basic load/store/alloca instructions" )
{
	do_file_compare_test (
			"test_cases/01_load_store_alloca.ll",
			"test_cases/01_load_store_alloca.nts"
	);
}


TEST_CASE ( "Call", "Call void and nonvoid function with and without parameters")
{
	do_file_compare_test (
			"test_cases/02_call.ll",
			"test_cases/02_call.nts"
	);
}

TEST_CASE ( "GlobalVars", "Read / write from / to global variables" )
{
	do_file_compare_test (
			"test_cases/03_global.ll",
			"test_cases/03_global.nts"
	);
}

TEST_CASE ( "PthreadCreate", "Process calls to pthread_create(3)" )
{
	do_file_compare_test ( "./test_cases/04_pthread_create.ll",
			               "./test_cases/04_pthread_create.nts" );
}
