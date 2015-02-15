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

const State * InstLoadStore::process(
		const NTS::State        * from    ,
		const llvm::Instruction & i       ,
		VariableManager         & vm      ,
		NTS::BasicNts           & n       ,
		int                       bb_id   ,
		int                       inst_id )
{
	const NTS::IPrint * src = NULL;
	const NTS::IPrint * dest = NULL;

	const NTS::State * st_to;

	switch(i.getOpcode())
	{
		case llvm::Instruction::Store:
			{
				const auto &st = llvm::cast<llvm::StoreInst>(i);
				src   = vm.getIPrint ( st.getValueOperand() );
				dest  = vm.getIPrint ( st.getPointerOperand() );
				st_to = & n.addState ( bb_id, inst_id );
			}
			break;

		case llvm::Instruction::Load:
			{
				const auto &ld = llvm::cast<llvm::LoadInst>(i);
				src   = vm.getIPrint ( ld.getPointerOperand() );
				dest  = vm.getIPrint ( &i );
				st_to = & n.addState ( bb_id, inst_id );
			}
			break;

		case llvm::Instruction::Ret:
			{
				const auto &rt = llvm::cast<llvm::ReturnInst>(i);
				src   = vm.getIPrint ( rt.getReturnValue() );
				dest  = n.getRetVar ( );
				st_to = & n.addFinalState( bb_id, inst_id );
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
	n.addTransition (from, st_to, cf);

	return st_to;
}

