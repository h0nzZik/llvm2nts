/**
 * @file InstIcmp. hpp
 * @date 28. 2. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 *
 * Implements 'icmp' instruction.
 */

#include <stdexcept>
#include "InstIcmp.hpp"

using namespace NTS;

InstIcmp::InstIcmp ( Constants & constants ) :
	m_constants   ( constants ),
	m_av_result   ( 0, true  ),
	m_av_op1      ( 1, false ),
	m_av_op2      ( 2, false ),
	m_av_sigbound ( 3, false ),
	m_av_true     ( 4, false ),
	m_av_false    ( 5, false ),
	m_havoc       ( {0} ),

	m_op1_negative    ( AtomicRelation::Relation::Gt,  &m_av_op1, &m_av_sigbound ),
	m_op1_nonnegative ( AtomicRelation::Relation::Le,  &m_av_op1, &m_av_sigbound ),
	m_op2_negative    ( AtomicRelation::Relation::Gt,  &m_av_op2, &m_av_sigbound ),
	m_op2_nonnegative ( AtomicRelation::Relation::Le,  &m_av_op2, &m_av_sigbound ),

	m_neg_equiv          ( BoolOp::Equiv, &m_op1_negative, &m_op2_negative    ),
	m_op1_neg_op2_nonneg ( BoolOp::And,   &m_op1_negative, &m_op2_nonnegative ),
	m_op2_neg_op1_nonneg ( BoolOp::And,   &m_op2_negative, &m_op1_nonnegative ),

	m_op1_eq_op2  ( AtomicRelation::Relation::Eq,  &m_av_op1, &m_av_op2 ),
	m_op1_neq_op2 ( AtomicRelation::Relation::Neq, &m_av_op1, &m_av_op2 ),

	m_op1_ult_op2 ( AtomicRelation::Relation::Lt,  &m_av_op1, &m_av_op2 ),
	m_op1_ule_op2 ( AtomicRelation::Relation::Le,  &m_av_op1, &m_av_op2 ),
	m_op1_uge_op2 ( AtomicRelation::Relation::Ge,  &m_av_op1, &m_av_op2 ),
	m_op1_ugt_op2 ( AtomicRelation::Relation::Gt,  &m_av_op1, &m_av_op2 ),

	m_neg_equiv_and_op1_lt_op2 ( BoolOp::And, &m_neg_equiv, &m_op1_ult_op2 ),
	m_neg_equiv_and_op1_gt_op2 ( BoolOp::And, &m_neg_equiv, &m_op1_ugt_op2 ),

	m_op1_slt_op2 ( BoolOp::Or, &m_neg_equiv_and_op1_lt_op2, &m_op1_neg_op2_nonneg ),
	m_op1_sgt_op2 ( BoolOp::Or, &m_neg_equiv_and_op1_gt_op2, &m_op2_neg_op1_nonneg ),

	m_op1_sle_op2 ( BoolOp::Or, &m_op1_slt_op2, &m_op1_eq_op2 ),
	m_op1_sge_op2 ( BoolOp::Or, &m_op1_sgt_op2, &m_op1_eq_op2 ),

	m_assign_true  ( AtomicRelation::Relation::Eq,  &m_av_result, &m_av_true  ),
	m_assign_false ( AtomicRelation::Relation::Eq,  &m_av_result, &m_av_false ),

	m_eq_assign_true   ( BoolOp::Impl, &m_op1_eq_op2,  &m_assign_true  ),
	m_neq_assign_false ( BoolOp::Impl, &m_op1_neq_op2, &m_assign_false ),

	m_ult_assign_true  ( BoolOp::Impl, &m_op1_ult_op2, &m_assign_true  ),
	m_ule_assign_true  ( BoolOp::Impl, &m_op1_ule_op2, &m_assign_true  ),

	m_ugt_assign_false ( BoolOp::Impl, &m_op1_ugt_op2, &m_assign_false ),
	m_uge_assign_false ( BoolOp::Impl, &m_op1_uge_op2, &m_assign_false ),

	m_slt_assign_true  ( BoolOp::Impl, &m_op1_slt_op2, &m_assign_true  ),
	m_sle_assign_true  ( BoolOp::Impl, &m_op1_sle_op2, &m_assign_true  ),

	m_sgt_assign_false ( BoolOp::Impl, &m_op1_sgt_op2, &m_assign_false  ),
	m_sge_assign_false ( BoolOp::Impl, &m_op1_sge_op2, &m_assign_false  ),

	m_f_eq  ( BoolOp::And, &m_eq_assign_true,  &m_neq_assign_false  ),
	m_f_ult ( BoolOp::And, &m_ult_assign_true, &m_uge_assign_false  ),
	m_f_ule ( BoolOp::And, &m_ule_assign_true, &m_ugt_assign_false  ),
	m_f_slt ( BoolOp::And, &m_slt_assign_true, &m_sge_assign_false  ),
	m_f_sle ( BoolOp::And, &m_sle_assign_true, &m_sgt_assign_false  ),

	m_f_eq_havoc  ( BoolOp::And, &m_f_eq,  &m_havoc ),
	m_f_ult_havoc ( BoolOp::And, &m_f_ult, &m_havoc ),
	m_f_ule_havoc ( BoolOp::And, &m_f_ule, &m_havoc ),
	m_f_slt_havoc ( BoolOp::And, &m_f_slt, &m_havoc ),
	m_f_sle_havoc ( BoolOp::And, &m_f_sle, &m_havoc )
{
	;
}

InstIcmp::~InstIcmp()
{
	
}

bool InstIcmp::supports(unsigned int opcode) const
{
	return opcode == llvm::Instruction::ICmp;
}

ConcreteFormula InstIcmp::get_formula (
		llvm::ICmpInst::Predicate   p,
		const IPrint              * result,
		const IPrint              * op_1,
		const IPrint              * op_2,
		const IPrint              * signed_bound)
{
	const std::initializer_list<const IPrint *>  &il_1 =
	{
			result,
			op_1,
			op_2,
			signed_bound,
			&m_constants.bool_true,
			&m_constants.bool_false
	};

	// Has swapped operands
	const std::initializer_list<const IPrint *>  &il_2 =
	{
			result,
			op_2,
			op_1,
			signed_bound,
			&m_constants.bool_true,
			&m_constants.bool_false
	};

	switch ( p )
	{
	case llvm::ICmpInst::ICMP_EQ:
		return ConcreteFormula ( m_f_eq_havoc,  il_1 );

	case llvm::ICmpInst::ICMP_ULE:
		return ConcreteFormula ( m_f_ule_havoc, il_1 );

	case llvm::ICmpInst::ICMP_SLE:
		return ConcreteFormula ( m_f_sle_havoc, il_1 );

	case llvm::ICmpInst::ICMP_ULT:
		return ConcreteFormula ( m_f_ult_havoc, il_1 );

	case llvm::ICmpInst::ICMP_SLT:
		return ConcreteFormula ( m_f_slt_havoc, il_1 );

	case llvm::ICmpInst::ICMP_NE:
		return ConcreteFormula ( m_f_eq_havoc,  il_2 );

	case llvm::ICmpInst::ICMP_UGT:
		return ConcreteFormula ( m_f_ule_havoc, il_2 );

	case llvm::ICmpInst::ICMP_UGE:
		return ConcreteFormula ( m_f_ult_havoc, il_2 );

	case llvm::ICmpInst::ICMP_SGT:
		return ConcreteFormula ( m_f_sle_havoc, il_2 );

	case llvm::ICmpInst::ICMP_SGE:
		return ConcreteFormula ( m_f_slt_havoc, il_2 );

	default:
		throw std::logic_error ( "ICMP: unknown predicate" );
	}
}

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
	const NTS::State * st_to  = n.addState ( bb_id, inst_id );
	n.addTransition ( from, st_to, cf );
	return st_to;
}
