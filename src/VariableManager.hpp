#ifndef _VARIABLEMANAGER_HPP_
#define _VARIABLEMANAGER_HPP_

#include "llvm/ADT/ValueMap.h"
#include "llvm/ADT/DenseMap.h"
#include "nts/NTS.hpp"
#include "nts/AbstractArithmetic.hpp"
#include "nts/IPrint.hpp"

// Manages variables and constants
class VariableManager final
{
	private:
		llvm::ValueMap<const llvm::Value *, NTS::IPrint *> m_variables;

#if 0
		typedef llvm::DenseMap<const NTS::Variable *,
				NTS::ArithmeticVariableIdentifier *> dm_t;

		dm_t m_var_to_arith_unprimed;
		dm_t m_var_to_arith_primed;
#endif
	public:
		VariableManager() {;}
		~VariableManager();

		void insVariable(const llvm::Value *llva, NTS::IPrint *var);
		NTS::IPrint * getIPrint(const llvm::Value * llval);
#if 0
		NTS::ArithmeticVariableIdentifier * getArithPrimed(const NTS::Variable *var);
		NTS::ArithmeticVariableIdentifier * getArithUnprimed(const NTS::Variable *var);
#endif
};










#endif // _VARIABLEMANAGER_HPP_
