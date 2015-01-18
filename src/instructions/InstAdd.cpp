#include <stdexcept>
#include <sstream>

#include <llvm/Support/Casting.h> // llvm::cast
#include "InstAdd.hpp"

using namespace NTS;

// All formula templates takes these parameters:
// 0 => destination
// 1 => left source
// 2 => right source
// 3 => unsigned bound
// 4 => signed bound
// More, all formulas exists in all bit versions,
// starting from.. 0bit?
// TODO: Test what happens with 0b, 1b or 2b formulas

InstAdd::InstAdd()
	:
		m_adst(0, true),
		m_als(1, false),
		m_ars(2, false),

		m_aub(3, false),
		m_asb(4, false),

		m_plus(AbstArithTermRelation::Connector::Add, &m_als, &m_ars),
		m_plus_mod(AbstArithTermRelation::Connector::Mod, &m_plus, &m_aub),
		m_ub_plus_sb(AbstArithTermRelation::Connector::Add, &m_aub, &m_asb),

		m_ls_pos(AtomicRelation::Relation::Lt, &m_als, &m_asb),
		m_rs_pos(AtomicRelation::Relation::Lt, &m_ars, &m_asb),
		m_srcs_pos(BoolOp::And, &m_ls_pos, &m_rs_pos),

		m_ls_neg(AtomicRelation::Relation::Ge, &m_als, &m_asb),
		m_rs_neg(AtomicRelation::Relation::Ge, &m_ars, &m_asb),
		m_srcs_neg(BoolOp::And, &m_ls_neg, &m_rs_neg),

		m_plus_ge_sb(AtomicRelation::Relation::Ge, &m_plus, &m_asb),
		m_plus_ge_ub(AtomicRelation::Relation::Ge, &m_plus, &m_aub),
		m_plus_lt_sb_ub(AtomicRelation::Relation::Lt, &m_plus, &m_ub_plus_sb),

		m_f_signed_of_positive(BoolOp::And, &m_srcs_pos, &m_plus_ge_sb),
		m_f_signed_of_negative(BoolOp::And, &m_srcs_neg, &m_plus_lt_sb_ub),
		m_f_signed_of(BoolOp::Or, &m_f_signed_of_positive, &m_f_signed_of_negative),
		m_f_no_signed_of(&m_f_signed_of),

		m_f_no_unsigned_of(AtomicRelation::Relation::Lt, &m_plus, &m_aub),
		m_f_no_of(BoolOp::And, &m_f_no_signed_of, &m_f_no_unsigned_of),

		m_f_assign(AtomicRelation::Relation::Eq, &m_adst, &m_plus_mod),

		m_f(m_f_assign),
		m_fs(BoolOp::Impl, &m_f_no_signed_of, &m_f_assign),
		m_fu(BoolOp::Impl, &m_f_no_unsigned_of, &m_f_assign),
		m_fsu(BoolOp::Impl, &m_f_no_of, &m_f_assign)
{

	;

}

const InstAdd::BitsizeGroup & InstAdd::getBitsizeGroup(unsigned int bitsz)
{
	// Only bit size of 2 to 63 bits is supported
	// TODO: add support for greater scales (eg using APInt)
	if (bitsz < 2 || bitsz >= 8 * sizeof(unsigned long long int))
		throw std::domain_error("Bit width too large");

	if (m_bs.size() <= bitsz)
		m_bs.resize(bitsz + 1);

	BitsizeGroup &g = m_bs[bitsz];

	// If one of them are NULL, all of them are NULL
	if (!g.unsigned_bound)
	{
		std::stringstream ss;
		unsigned long long int x = 1ULL << bitsz;
		ss << x;
		g.unsigned_bound = new ConPositiveNumeral(ss.str());
		ss.str(std::string());
		x = 1ULL << (bitsz - 1);
		ss << x;

		// assert(g.signed_bound == NULL);
		g.signed_bound = new ConPositiveNumeral(ss.str());
	}

	return g;
}

bool InstAdd::supports(unsigned int opcode) const
{
	return opcode == llvm::Instruction::Add;
}

const NTS::Formula & InstAdd::getFormula(bool signed_wrap, bool unsigned_wrap) const
{
	if (!signed_wrap && !unsigned_wrap)
		return m_f;
	if (!signed_wrap && unsigned_wrap)
		return m_fu;
	if (signed_wrap && !unsigned_wrap)
		return m_fs;
	return m_fsu;
}

NTS::ConcreteFormula InstAdd::process(const llvm::Instruction &i, VariableManager &vm)
{
	if (i.getOpcode() != llvm::Instruction::Add)
		throw std::invalid_argument("Unknown llvm instruction");

	auto &bo = llvm::cast<llvm::BinaryOperator>(i);

	const IPrint *left = vm.getIPrint(bo.getOperand(0));
	const IPrint *right = vm.getIPrint(bo.getOperand(1));
	const IPrint *result = vm.getIPrint(&llvm::cast<llvm::Value>(i));
	auto &g = getBitsizeGroup(i.getType()->getIntegerBitWidth());

	const NTS::Formula &f = getFormula(!bo.hasNoSignedWrap(), !bo.hasNoUnsignedWrap());

	return NTS::ConcreteFormula(f,
			{result, left, right, g.unsigned_bound, g.signed_bound});
}
