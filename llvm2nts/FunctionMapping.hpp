#ifndef SRC_FUNCTION_MAPPING_HPP_
#define SRC_FUNCTION_MAPPING_HPP_

#include <memory>

#include <llvm/IR/Value.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/ADT/DenseMap.h>

#include <libNTS/nts.hpp>
#include <libNTS/logic.hpp>

#include "ModuleMapping.hpp"


struct StateInfo
{
	nts::State             * st;
	const llvm::BasicBlock * bb;
	unsigned int bb_id;
	unsigned int inst_id;

	StateInfo ( nts::State &st, const llvm::BasicBlock & bb ) :
		st ( &st ), bb ( &bb )
	{
		;
	}

	StateInfo ( const StateInfo & orig ) :
		st      ( orig.st ),
		bb      ( orig.bb ),
		bb_id   ( orig.bb_id ),
		inst_id ( orig.inst_id )
	{
		;
	}

	StateInfo & operator= ( const StateInfo & orig )
	{
		st = orig.st;
		bb = orig.bb;
		bb_id = orig.bb_id;
		inst_id = orig.inst_id;
		return *this;
	}
};


class FunctionMapping
{
	private:
		nts::BasicNts & m_nts;
		llvm::DenseMap <const llvm::BasicBlock *, StateInfo * > m_block_start;

		llvm::DenseMap <
			const llvm::Value *,
			nts::Variable * > m_vars;

		// Maps (local) values of pointer types
		// to local variables.
		llvm::DenseMap <
			const llvm::Value *,
			nts::Variable * > m_pointers;

		nts::Variable * get_variable_noexcept ( const llvm::Value & value ) const noexcept;
		nts::Variable * get_variable_by_pointer_noexcept
			( const llvm::Value & value ) const noexcept;

	public:
		const ModuleMapping & m_modmap;

		FunctionMapping ( nts::BasicNts & nts, const ModuleMapping &mod );
		~FunctionMapping();

		/**
		 * @brief Creates new variable and inserts it into mapping.
		 * Caller would probably want to insert the variable
		 * to nts as an argument or as an variable.
		 * Assumes there is no variable with the same name.
		 */
		std::unique_ptr<nts::Variable> new_variable ( const llvm::Value & llval );
		void ins_variable ( const llvm::Value & llval, nts::Variable & var );

		// must be pointer type
		void ins_pointer ( const llvm::Value & value, nts::Variable & var );

		nts::Variable & get_variable ( const llvm::Value & value ) const;
		nts::Variable & get_variable_by_pointer ( const llvm::Value & value ) const;

		
		//const NTS::IPrint * get_iprint ( int n );
		//const NTS::IPrint * get_iprint ( const llvm::Value *value );

		void ins_bb_start ( const llvm::BasicBlock & block,
						    std::unique_ptr<StateInfo> s );

		StateInfo & get_bb_start ( const llvm::BasicBlock & block ) const;

		const decltype ( m_block_start ) & bbinfo() const { return m_block_start; }

		/*
		 * @brief Creates new read-only Leaf (Constant or VariableReference)
		 * @param value - llvm value, interpreted as normal non-pointer value.
		 *                Currently can be only of Integer type.
		 *                Leafs from pointer types (e.g. alloca )
		 *                can be created using get_variable_by_pointer()
		 *                and VariableReference.
		 */
		std::unique_ptr < nts::Leaf > new_leaf ( const llvm::Value & value ) const;

		/**
		 * @brief Create leaf with type BitVector<1> (aka Bool)
		 */
		std::unique_ptr < nts::Leaf > new_boolleaf ( const llvm::Value & value ) const;


		// New primed variable reference
		std::unique_ptr < nts::VariableReference >
			new_primed ( const llvm::Value & value ) const;
};


#endif // SRC_FUNCTION_MAPPING_HPP_
