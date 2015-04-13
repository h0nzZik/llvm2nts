#ifndef SRC_LLVM_FUNCTION_TO_NTS_HPP
#define SRC_LLVM_FUNCTION_TO_NTS_HPP

#include <llvm/ADT/DenseMap.h>

#include "nts/NTS.hpp"

#include "FunctionMapping.hpp"

class llvmFunction2nts
{
	private:
		const llvm::Function & m_f;
		FunctionMapping        m_map;
		NTS::BasicNts        & m_nts;

		int m_bb_id;
		int m_inst_id;

		void process_basic_block ( const llvm::BasicBlock & b );

		const NTS::State * process_instruction (
				const NTS::State        * st_from ,
				const llvm::Instruction & i       );

		void process_basic_block ( 
				const llvm::BasicBlock & b,
				int                      bb_id );

		const NTS::Variable * add_param ( const llvm::Argument * arg );

	public:
		llvmFunction2nts ( const llvm::Function & f,
						   NTS::BasicNts        & nts,
						   const ModuleMapping  & modmap );
		~llvmFunction2nts ( );

		void process();

};

void convert_function ( const llvm::Function & f, BasicNtsInfo & bni );

#endif // SRC_LLVM_FUNCTION_TO_NTS_HPP

