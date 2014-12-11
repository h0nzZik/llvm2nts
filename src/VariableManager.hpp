#ifndef _VARIABLEMANAGER_HPP_
#define _VARIABLEMANAGER_HPP_

#include "llvm/ADT/ValueMap.h"
#include "llvm/ADT/DenseMap.h"
#include "nts/NTS.hpp"
#include "nts/Arithmetic.hpp"

class VariableManager
{
	private:
		llvm::ValueMap<const llvm::Value *, NTS::Variable *> m_variables;

		typedef llvm::DenseMap<const NTS::Variable *,
				NTS::ArithmeticVariableIdentifier *> dm_t;

		dm_t m_var_to_arith_unprimed;
		dm_t m_var_to_arith_primed;

	public:
		VariableManager() {;}
		~VariableManager();

		void insVariable(const llvm::Value *llva, NTS::Variable *var);
		NTS::Variable * getVariable(const llvm::Value * llval);
		NTS::ArithmeticVariableIdentifier * getArithPrimed(const NTS::Variable *var);
		NTS::ArithmeticVariableIdentifier * getArithUnprimed(const NTS::Variable *var);
};










#endif // _VARIABLEMANAGER_HPP_
