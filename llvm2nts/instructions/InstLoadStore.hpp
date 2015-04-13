#ifndef _INSTRUCTION_INST_LOAD_STORE_HPP_
#define _INSTRUCTION_INST_LOAD_STORE_HPP_

// LLVM headers
#include <llvm/IR/Instructions.h>

// NTS headers
#include <nts/AbstractArithmetic.hpp>
#include <nts/ConcreteArithmetic.hpp>
#include <nts/AtomicRelation.hpp>
#include <nts/Havoc.hpp>

// Project headers
#include "IInst.hpp"

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

		virtual void process (
				StateInfo               & sti,
				FunctionMapping         & map,
				const llvm::Instruction & i    ) override;
};


#endif // _INSTRUCTION_INST_LOAD_STORE_HPP_

