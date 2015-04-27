/**
 * @file InstIcmp. hpp
 * @date 28. 2. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'icmp' instruction.
 */

#include <stdexcept>
#include <llvm/Support/Casting.h> // llvm::cast

#include <libNTS/logic.hpp>
#include <libNTS/nts.hpp>
#include <libNTS/sugar.hpp>

#include "../util.hpp"
#include "InstIcmp.hpp"


using namespace nts;
using namespace llvm;
using std::unique_ptr;

using namespace sugar;

unique_ptr < Formula > InstIcmp::predicate (
		CmpInst::Predicate p,
		unique_ptr < Term > left,
		unique_ptr < Term > right ) const
{
	// see sugar.hpp
	switch ( p )
	{
		case ICmpInst::ICMP_EQ:
			return left == right;

		case ICmpInst::ICMP_ULT:
			return left < right;

		case ICmpInst::ICMP_ULE:
			return left <= right;

		case ICmpInst::ICMP_UGT:
			return left > right;

		case ICmpInst::ICMP_UGE:
			return left >= right;

		case ICmpInst::ICMP_NE:
			return left != right;

		case ICmpInst::ICMP_SGE:
		case ICmpInst::ICMP_SLE:
		case ICmpInst::ICMP_SGT:
		case ICmpInst::ICMP_SLT:
			throw std::domain_error ( "Signed comparison is not implemented" );

		default:
			throw std::domain_error ( "Unknown ICMP operator" );
	}
}

void InstIcmp::process (
		const BasicNtsInfo      & bntsi,
		StateInfo               & sti,
		FunctionMapping         & map,
		const llvm::Instruction & i    )
{
	auto st_next = new_state ( sti.bb_id, sti.inst_id );
	st_next->insert_after ( *sti.st );

	if (i.getOpcode() != Instruction::ICmp)
		throw std::invalid_argument("Unsupported llvm instruction");

	const auto &icmp     = cast < llvm::ICmpInst > ( i );

	unique_ptr < Leaf > left  = map.new_leaf ( * icmp.getOperand ( 0 ) );
	unique_ptr < Leaf > right = map.new_leaf ( * icmp.getOperand ( 1 ) );



	unique_ptr < Formula > condition = predicate (
			icmp.getPredicate(),
			move ( left  ),
			move ( right )
	);

	Variable * result = map.new_variable( icmp ).release();
	result->insert_to ( bntsi.bn );
	Havoc &h = * new Havoc( { result } );

	// Should be of type 'Bool' or 'BitVector<1>'
	auto one = std::make_unique<IntConstant> ( 1 );
	auto is_true = std::make_unique<Relation> (
			RelationOp::eq,
			std::make_unique<VariableReference> ( *result, true ),
			move ( one )
	);

	Formula &eq = *(condition.release()) == *(is_true.release());

	Formula &f = eq && h;

	auto ftr = std::make_unique < FormulaTransitionRule >
		( unique_ptr < Formula > ( &f ) );
	auto * tr  = new Transition ( move ( ftr ), *sti.st, *st_next );
	tr->insert_to ( bntsi.bn );
	sti.st = st_next;
}

#if 0
const State * InstIcmp::process(
		const NTS::State        * from    ,
		const llvm::Instruction & i       ,
		FunctionMapping         & map     ,
		NTS::BasicNts           & n       ,
		int                       bb_id   ,
		int                       inst_id )
{
	if (i.getOpcode() != llvm::Instruction::ICmp)
		throw std::invalid_argument("Unsupported llvm instruction");

	const auto &icmp     = llvm::cast < llvm::ICmpInst > ( i );
	const IPrint *left   = map.get_iprint ( icmp.getOperand(0) );
	const IPrint *right  = map.get_iprint ( icmp.getOperand(1) );
	const IPrint *result = map.get_iprint ( &llvm::cast<llvm::Value>(i) );

	unsigned int  width  = i.getType()->getIntegerBitWidth();
	const Constants::BitsizeGroup &g  = m_constants.get_bitsize_group ( width );

	ConcreteFormula cf        = get_formula ( icmp.getPredicate(), result, left, right, g.signed_bound );
	const TransitionRule *r   = n.add_transition_rule ( cf );
	const NTS::State * st_to  = n.addState ( bb_id, inst_id );
	n.add_transition ( from, st_to, r );
	return st_to;
}

#endif
