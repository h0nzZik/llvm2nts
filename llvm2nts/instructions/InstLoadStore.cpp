#include "InstLoadStore.hpp"

using namespace nts;
using namespace llvm;

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

void InstLoadStore::process (	
		StateInfo         & sti,
		FunctionMapping   & map,
		const Instruction & i    )
{
	switch ( i.getOpcode() )
	{
		case llvm::Instruction::Store:
		{
			auto &st          = cast < StoreInst > ( i );
			const auto & dest = map.get_variable ( *st.getPointerOperand() );
			auto leaf         = map.new_leaf ( *st.getValueOperand() );
			break;
		}

		default:

			break;
	}
}

const State * InstLoadStore::process(
		const NTS::State        * from    ,
		const llvm::Instruction & i       ,
		FunctionMapping         & map     ,
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
				src   = map.get_iprint ( st.getValueOperand() );
				dest  = map.get_iprint ( st.getPointerOperand() );
				st_to = n.addState ( bb_id, inst_id );
			}
			break;

		case llvm::Instruction::Load:
			{
				const auto &ld = llvm::cast<llvm::LoadInst>(i);
				src   = map.get_iprint ( ld.getPointerOperand() );
				dest  = map.get_iprint ( &i );
				st_to = n.addState ( bb_id, inst_id );
			}
			break;

		case llvm::Instruction::Ret:
			{
				const auto &rt = llvm::cast<llvm::ReturnInst>(i);
				st_to = n.final_state();
				dest  = n.get_return_variable ( 0 );

				// If there is no return value, just havoc()
				if ( !rt.getReturnValue() )
				{
					// Our havoc accesses only 0th item
					ConcreteFormula cf ( m_havoc, { dest } );
					const TransitionRule *r = n.add_transition_rule ( cf );
					n.add_transition ( from, st_to, r );
					return st_to;
				}

				// Else move the return value to coresponding variable
				src   = map.get_iprint ( rt.getReturnValue() );
			}
			break;

		default:
			throw std::invalid_argument("Unsupported llvm instruction");
	}

	// assert(src && dest);
	ConcreteFormula cf(m_f, {dest, src});
	const TransitionRule *r = n.add_transition_rule ( cf );
	n.add_transition ( from, st_to, r );

	return st_to;
}

