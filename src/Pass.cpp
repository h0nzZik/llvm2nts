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

static cl::opt<std::string> OutputFilename(
		"llvm2nts_of",
		cl::desc("Specify output filename for llvm2nts"),
		cl::value_desc("output.nts"),
		cl::Required
		);

namespace {

	class LLVM2NTS : public ModulePass
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
				for (const auto &f : M.getFunctionList())
				{
					onFunction(f, ofs);
				}
				ofs.close();
				return false;
			}

		private:

			void onFunction(const Function &F, std::ostream &str)
			{
				llvm2nts l2n(F.getReturnType());

				str << F.getName().str() << "{\n";
				for (const auto &v : F.getArgumentList())
				{
					l2n.addParam(&v);
				}

				for (const auto &b : F.getBasicBlockList())
				{
					for (const auto &i : b.getInstList())
					{
						l2n.processInstruction(i);
					}
				}

				l2n.print(str);
			}
	};
}

char LLVM2NTS::ID = 0;
static RegisterPass<LLVM2NTS> X("llvm2nts", "Converts llvm IR to NTS", false, false);
