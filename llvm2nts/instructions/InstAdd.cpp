#include <stdexcept>
#include <sstream>

#include <llvm/Support/Casting.h> // llvm::cast
#include <libNTS/logic.hpp>
#include <libNTS/nts.hpp>

#include "../util.hpp"
#include "InstAdd.hpp"

using namespace nts;

void InstAdd::process (
		const BasicNtsInfo      & bntsi,
		StateInfo               & sti,
		FunctionMapping         & map,
		const llvm::Instruction & i    )
{
	auto st_next = new_state ( sti.bb_id, sti.inst_id );
	st_next->insert_after ( *sti.st );

	if (i.getOpcode() != llvm::Instruction::Add)
		throw std::invalid_argument("Unknown llvm instruction");

	auto &bo = llvm::cast<llvm::BinaryOperator>(i);

	auto left  = map.new_leaf ( * bo.getOperand ( 0 ) );
	auto right = map.new_leaf ( * bo.getOperand ( 1 ) );
	auto result  = std::make_unique < ArithmeticOperation > (
			ArithOp::Add,
			move ( left ),
			move ( right )
	);

	auto & dest_var = * map.new_variable ( bo );
	dest_var.insert_to ( bntsi.bn );
	auto ref = std::make_unique < VariableReference > (
			dest_var,
			true
	);

	auto eq = std::make_unique < Relation > (
			RelationOp::eq,
			move ( ref ),
			move ( result )
	);

	auto havoc = new Havoc ( { &dest_var } );

	auto f = std::make_unique < FormulaBop > (
			BoolOp::And,
			move ( eq ),
			std::unique_ptr < Havoc > ( havoc )
	);

	auto r = std::make_unique < FormulaTransitionRule > ( move ( f ) );
	auto tr = new Transition ( move ( r ), *sti.st, *st_next );
	tr->insert_to ( bntsi.bn );
	sti.st = st_next;
	// TODO: Go to error state on overflow (if nsw / nuw is set )
	

}

