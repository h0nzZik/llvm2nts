#ifndef _INSTRUCTION_INST_LOAD_STORE_HPP_
#define _INSTRUCTION_INST_LOAD_STORE_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>

// Project headers
#include "IInst.hpp"
#include "../nts/AbstractArithmetic.hpp"
#include "../nts/ConcreteArithmetic.hpp"
#include "../nts/AtomicRelation.hpp"
#include "../nts/Havoc.hpp"

class InstLoadStore : public IInst
{
	private:
		NTS::AbstArithValue m_dst;
		NTS::AbstArithValue m_src;
		NTS::AtomicRelation m_assign;
		NTS::Havoc m_havoc;
		NTS::FormulaBop m_f;

	public:
		InstLoadStore();
		~InstLoadStore() {;}

		virtual bool supports(unsigned int opcode) const;

		virtual const NTS::State * process(
				const NTS::State        * from    ,
				const llvm::Instruction & i       ,
				FunctionMapping         & map     ,
				NTS::BasicNts           & n       ,
				int                       bb_id   ,
				int                       inst_id );
};


#endif // _INSTRUCTION_INST_LOAD_STORE_HPP_

