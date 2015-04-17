#include <iostream>

#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>

#include <llvm2nts/llvm2nts.hpp>

#include <libNTS/nts.hpp>


using std::cout;
using std::cerr;
using std::unique_ptr;

int main ( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		cout << "usage: " << argv[0] << " filename.ll\n";
		return 1;
	}

	const char * file = argv[1];

	llvm::SMDiagnostic diag;
	llvm::LLVMContext        ctx;

	unique_ptr<llvm::Module> m = llvm::parseIRFile ( file, diag, ctx );

	if ( !m )
	{
		std::cerr << "Error on line " << diag.getLineNo() << ":\n";
		std::cerr << diag.getMessage().str();
		return 1;
	}

	unique_ptr < nts::Nts > nts = llvm_to_nts ( *m );
	cout << *nts;

	return 0;

}
