#include <memory>
#include <string>

#include <libNTS/logic.hpp>

#include "InstLoadStore.hpp"

using namespace nts;
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
	auto st_next = new State (
			string ( "st_" ) +
			to_string ( sti.bb_id ) +
			string ( "_" ) +
			to_string ( sti.inst_id )
	);

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

			if ( ret.getReturnValue() )
			{
				dest_var = bntsi.ret_var;
				src      = map.new_leaf ( *ret.getReturnValue() );
			}
			break;
		}

		default:
			throw std::invalid_argument("Unsupported llvm instruction");
	}

	unique_ptr < Formula > formula;
	
	if ( dest_var && src )
	{
		auto i1      = { (const Variable *)dest_var };
		auto havoc   = std::make_unique < Havoc > ( i1 );
		auto dest    = std::make_unique < VariableReference > ( *dest_var, true );
		auto assign  = std::make_unique < Relation > (
						RelationOp::eq, move ( src ), move ( dest ) );
		formula      = std::make_unique < FormulaBop > (
						BoolOp::And, move ( assign ), move ( havoc ) );
	}
	else if ( !dest_var && !src )
	{
		auto i1 = std::initializer_list < const Variable *> {};
		formula = std::make_unique < Havoc > ( i1 );
	} else {
		throw std::logic_error ( "condition does not hold: dest_var <==> src" );
	}

	auto rule = std::make_unique < FormulaTransitionRule > ( move ( formula ) );

	auto transition = new Transition ( move ( rule ), *sti.st, *st_next );
	(void)transition;
	sti.st = st_next;
}

