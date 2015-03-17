#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <catch.hpp>

#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>

#include <nts/NtsModule.hpp>

#include <llvm2nts/llvm2nts.hpp>



using namespace NTS;
using namespace llvm;

static void do_file_compare_test ( const char *llfile,
		const char *ntsfile,
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
	NtsModule nm;
	llvm2nts conv ( nm, *m);
	try {
		conv.process();
		nm.print ( gen_ss );
		if ( print )
			std::cout << gen_ss.str();
	} catch ( std::exception &e ) {
		std::cerr << "An exception was thrown while processing:\n" << e.what();
		FAIL ("An exception was thrown while processing");
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

TEST_CASE ( "LoadStoreAlloca", "Tests basic load/store/alloca instructions" )
{
	do_file_compare_test ( "test_cases/01_load_store_alloca.ll", "test_cases/01_load_store_alloca.nts" );
}

TEST_CASE ( "VoidFunction", "Simple void function")
{
	do_file_compare_test ( "test_cases/00_empty.ll", "test_cases/00_empty.nts");
}

TEST_CASE ( "Call", "Call void and nonvoid function with and without parameters")
{
	do_file_compare_test ( "test_cases/02_call.ll", "test_cases/02_call.nts");
}
