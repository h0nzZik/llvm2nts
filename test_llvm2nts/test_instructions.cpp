#include <iostream>
#include <sstream>
#include <stdexcept>

#include <catch.hpp>

#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>

#include <nts/NTS.hpp>

#include <llvm2nts/llvm2nts.hpp>



using namespace NTS;
using namespace llvm;

static void do_file_compare_test ( const char *llfile, const char *ntsfile )
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

	std::stringstream oss;
	llvm2nts conv;
	try {
		conv.process_module ( *m );
		conv.print ( oss );
	} catch ( std::exception &e ) {
		std::cerr << "An exception was thrown while processing:\n" << e.what();
		FAIL ("An exception was thrown while processing");
	}
	std::cout << oss.str();

}

TEST_CASE ( "LoadStoreAlloca", "Tests basic load/store/alloca instructions" )
{
	do_file_compare_test ( "test_cases/01_load_store_alloca.ll", nullptr );
}

TEST_CASE ( "VoidFunction", "Simple void function")
{

}

