#include <sstream>
#include <llvm/Support/Casting.h>
#include <llvm/IR/GlobalValue.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/Casting.h"

#include "VariableManager.hpp"



void VariableManager::insVariable(const llvm::Value *llval, NTS::IPrint *var)
{
	m_variables.insert(std::make_pair(llval, var));
}

NTS::IPrint * VariableManager::getIPrint(const llvm::Value *llval)
{
	NTS::IPrint *v = m_variables.lookup(llval);
	if (v)
		return v;

	if (llvm::isa<llvm::ConstantInt>(llval))
	{
		const auto *c =llvm:: cast<llvm::ConstantInt>(llval);
		const auto &v = c->getValue();

		// TODO: Make this more elegant
		std::string s;
		llvm::raw_string_ostream os(s);
		v.print(os, false);
		auto * co = new NTS::Constant(s);

		insVariable(llval, co);
		return co;
	}


	std::stringstream ss;
	if (llvm::isa<llvm::GlobalValue>(llval))
	{
		ss << "g_" << llval->getName().str();
	} else {
		ss << "l_" << llval->getName().str();
	}

	v = new NTS::Variable(ss.str());
	insVariable(llval, v);
	return v;
}

#if 0

NTS::ArithmeticVariableIdentifier *
VariableManager::getArithPrimed(const NTS::Variable *var)
{
	auto id = m_var_to_arith_primed.lookup(var);
	if (id)
		return id;

	id = new NTS::ArithmeticVariableIdentifier(var, true);
	m_var_to_arith_primed.insert(std::make_pair(var, id));
	return id;
}

NTS::ArithmeticVariableIdentifier *
VariableManager::getArithUnprimed(const NTS::Variable *var)
{
	auto id = m_var_to_arith_primed.lookup(var);
	if (id)
		return id;

	id = new NTS::ArithmeticVariableIdentifier(var, true);
	m_var_to_arith_unprimed.insert(std::make_pair(var,id));
	return id;
}
#endif

VariableManager::~VariableManager()
{
#if 0
	for (auto i : this->m_var_to_arith_primed)
	{
		delete i.second;
		i.second = NULL;
	}

	for (auto i : this->m_var_to_arith_unprimed)
	{
		delete i.second;
		i.second = NULL;
	}
#endif

	for (auto i : this->m_variables)
	{
		delete i.second;
		i.second = NULL;
	}
}
