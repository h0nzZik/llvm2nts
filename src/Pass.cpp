#include "llvm/Pass.h"
// Note: In version 3.5 ValueMap is inside IR directory
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/ValueMap.h"
#include "llvm/IR/Function.h"
// Note: In version 3.5 InstIterator is inside IR directory
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"

#include <sstream>

#include "nts/NTS.hpp"
#include "llvm2nts.hpp"

using namespace llvm;


// TODO: use instruction namer (instnamer) before this
namespace {

	struct Hello : public FunctionPass {
		static char ID;
		Hello() : FunctionPass(ID) {}
		
		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesAll();
			AU.addRequiredID(InstructionNamerID);
		}

		virtual bool runOnFunction(Function &F) {
			llvm2nts l2n(F.getReturnType());

			errs().write_escaped(F.getName()) << "{\n";
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

			std::stringstream s;
			l2n.print(s);
			errs() << "From stream:\n" << s.str();

			return false;
		}

	};

}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);
