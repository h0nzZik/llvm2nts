#include <memory>
#include <string>

#include <llvm/IR/Constants.h>

#include <libNTS/logic.hpp>
#include <libNTS/sugar.hpp>

#include "../util.hpp"
#include "InstLoadStore.hpp"

using namespace nts;
using namespace nts::sugar;
using namespace llvm;

using std::unique_ptr;
using std::make_unique;
using std::move;
using std::string;
using std::to_string;

void InstLoadStore::process (
		const BasicNtsInfo & bntsi,
		StateInfo          & sti,
		FunctionMapping    & map,
		const Instruction  & i    )
{
	auto st_next = new_state ( sti.bb_id, sti.inst_id );

	st_next->insert_after ( *sti.st );

	unique_ptr < Leaf >  src;
	Variable           * dest_var = nullptr;

	switch ( i.getOpcode() )
	{
		case llvm::Instruction::Store:
		{
			auto & st  = cast < StoreInst > ( i );
			dest_var   = & map.get_variable_by_pointer ( * st.getPointerOperand() );
			src        = map.new_leaf ( *st.getValueOperand() );
			break;
		}

		case llvm::Instruction::Load:
		{
			auto &ld      = cast < LoadInst > ( i );
			auto &src_var = map.get_variable_by_pointer ( * ld.getPointerOperand() );
			src           = std::make_unique < VariableReference > ( src_var, false );
			auto var      = map.new_variable ( ld );
			var->insert_to ( bntsi.bn );
			dest_var      = var.release() ;
			break;
		}

		case llvm::Instruction::Ret:
		{
			auto & ret = cast < ReturnInst > ( i );
			dest_var   = bntsi.ret_var;
			st_next->is_final() = true;

			if ( ret.getReturnValue() )
			{
				// thread functions does not return value
				// (i.e. they should always return null)
				if ( bntsi.is_ptf )
				{
					if ( !isa < llvm::ConstantPointerNull> ( ret.getReturnValue() ) )
						throw std::domain_error ( "Thread functions should return null" );
				} else {
					dest_var = bntsi.ret_var;
					src      = map.new_leaf ( *ret.getReturnValue() );
				}
			}
			break;
		}

		default:
			throw std::invalid_argument("Unsupported llvm instruction");
	}

	unique_ptr < Formula > formula;
	
	if ( dest_var && src )
	{
		Formula & f = ( NEXT ( dest_var ) == *src.release() ) && havoc ( {dest_var} );
		formula = unique_ptr < Formula > ( &f );
	}
	else if ( !dest_var && !src )
	{
		formula = std::make_unique < Havoc > ( );
	} else {
		throw std::logic_error ( "condition does not hold: dest_var <==> src" );
	}

	auto rule = std::make_unique < FormulaTransitionRule > ( move ( formula ) );

	auto transition = new Transition ( move ( rule ), *sti.st, *st_next );
	transition->insert_to ( bntsi.bn );
	sti.st = st_next;
}

