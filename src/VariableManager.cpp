#include <sstream>
#include <llvm/Support/Casting.h>
#include <llvm/IR/GlobalValue.h>
#include "VariableManager.hpp"



void VariableManager::insVariable(const llvm::Value *llval, NTS::Variable *var)
{
	m_variables.insert(std::make_pair(llval, var));
}

NTS::Variable * VariableManager::getVariable(const llvm::Value *llval)
{
	NTS::Variable *v = m_variables.lookup(llval);
	if (v)
		return v;

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

VariableManager::~VariableManager()
{
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

	for (auto i : this->m_variables)
	{
		delete i.second;
		i.second = NULL;
	}
}
