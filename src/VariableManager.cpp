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
		//v.print(os, false);
		os << v;
		auto * co = new NTS::Constant(os.str());

		insVariable(llval, co);
		return co;
	}


	std::stringstream ss;
	if (llvm::isa<llvm::GlobalValue>(llval))
	{
		ss << "g_";
	} else {
		ss << "l_";
	}

	std::string s = llval->getName().str();
	llvm::errs() << "name: " << s << "\n";
	ss << llval->getName().str();

	v = new NTS::Variable(ss.str());
	insVariable(llval, v);
	return v;
}

void VariableManager::ins_bb_start(const llvm::BasicBlock *block, const NTS::State *s)
{
	m_block_start.insert(std::make_pair(block, s));
}

const NTS::State * VariableManager::get_bb_start(const llvm::BasicBlock * block)
{
	auto * s = m_block_start.lookup(block);
	if (!s)
		throw std::logic_error("Start of basic block not found");
	return s;
}

VariableManager::~VariableManager()
{
	for (auto i : this->m_variables)
	{
		delete i.second;
		i.second = NULL;
	}
}

