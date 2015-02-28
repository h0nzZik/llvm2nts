#include <ostream>
#include <sstream>

#include <catch.hpp>

#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>

#include <nts/NTS.hpp>

#include <llvm2nts/FunctionMapping.hpp>



using namespace NTS;
using namespace llvm;


TEST_CASE( "Trivial", "Tests nothing")
{
	BasicNts nts ( "unnamed" );
	ModuleMapping modmap;
	FunctionMapping map ( nts, modmap );

	const State * s = nts.addState ( 0, 0 );

	auto buf = MemoryBuffer::getMemBuffer(StringRef(""));
	auto bufref = buf->getMemBufferRef();
	llvm::SMDiagnostic diag;
	LLVMContext ctx;
	std::unique_ptr<llvm::Module> m = llvm::parseIR(bufref, diag, ctx);
	
	REQUIRE(m);
	if (m)
	{

	}


	REQUIRE(0 == 0);
}

