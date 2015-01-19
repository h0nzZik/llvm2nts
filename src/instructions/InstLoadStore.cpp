#include "InstLoadStore.hpp"

using namespace NTS;

InstLoadStore::InstLoadStore()
	:
		m_dst(0, true),
		m_src(1, false),
		m_assign(AtomicRelation::Relation::Eq, &m_dst, &m_src),
		m_havoc({0}),
		m_f(BoolOp::And, &m_assign, &m_havoc)

{
	;
}

bool InstLoadStore::supports(unsigned int opcode) const
{
	switch(opcode)
	{
		case llvm::Instruction::Store:
			return true;

		case llvm::Instruction::Load:
			return true;

		case llvm::Instruction::Ret:
			return true;

		default:
			return false;
	}
}

ConcreteFormula InstLoadStore::process(
		const llvm::Instruction &i,
		VariableManager &vm,
		BasicNts &n)
{
	const NTS::IPrint * src = NULL;
	const NTS::IPrint * dest = NULL;

	switch(i.getOpcode())
	{
		case llvm::Instruction::Store:
			{
				const auto &st = llvm::cast<llvm::StoreInst>(i);
				src = vm.getIPrint(st.getValueOperand());
				dest = vm.getIPrint(st.getPointerOperand());
			}
			break;

		case llvm::Instruction::Load:
			{
				const auto &ld = llvm::cast<llvm::LoadInst>(i);
				src = vm.getIPrint(ld.getPointerOperand());
				dest = vm.getIPrint(&i);
			}
			break;

		case llvm::Instruction::Ret:
			{
				const auto &rt = llvm::cast<llvm::ReturnInst>(i);
				src = vm.getIPrint(rt.getReturnValue());
				dest = n.getRetVar();
				// TODO: Later we should support empty ret
				if (!dest || !src)
					throw std::logic_error("Ret without return value");
			}
			break;

		default:
			throw std::invalid_argument("Unsupported llvm instruction");
	}

	// assert(src && dest);
	ConcreteFormula cf(m_f, {dest, src});
	return cf;
}

