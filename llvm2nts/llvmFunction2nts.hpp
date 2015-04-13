#ifndef SRC_LLVM_FUNCTION_TO_NTS_HPP
#define SRC_LLVM_FUNCTION_TO_NTS_HPP

#include <llvm/ADT/DenseMap.h>

#include "FunctionMapping.hpp"

void convert_function (
		const llvm::Function & f,
		BasicNtsInfo         & bni,
		ModuleMapping        & mm );

#endif // SRC_LLVM_FUNCTION_TO_NTS_HPP

