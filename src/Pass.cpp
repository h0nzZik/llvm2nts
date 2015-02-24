#include <llvm/Pass.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/Support/raw_ostream.h>

#include <sstream>
#include <iostream>
#include <fstream>

#include "nts/NTS.hpp"
#include "llvm2nts.hpp"

using namespace llvm;

// FIXME local variables does not work!
// Probably problem with InstructionNamer pass?

static cl::opt<std::string> OutputFilename(
		"llvm2nts_of",
		cl::desc("Specify output filename for llvm2nts"),
		cl::value_desc("output.nts"),
		cl::Required
		);

namespace {

	class LLVM2NTS final : public ModulePass
	{
		public:
			static char ID;
			LLVM2NTS() : ModulePass(ID) {}

			virtual void getAnalysisUsage(AnalysisUsage &AU) const {
				AU.setPreservesAll();
				AU.addRequiredID(InstructionNamerID);
			}

			virtual bool runOnModule(Module &M)
			{
				std::ofstream ofs;
				ofs.open(OutputFilename.c_str());

				llvm2nts l2n;
				l2n.process_module ( M );

				l2n.print ( ofs );
				ofs.close();

				return false;
			}
	};
}

char LLVM2NTS::ID = 0;
static RegisterPass<LLVM2NTS> X("llvm2nts", "Converts llvm IR to NTS", false, false);
