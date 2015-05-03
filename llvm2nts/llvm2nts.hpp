#ifndef _LLVM2NTS_HPP_
#define _LLVM2NTS_HPP_

// C++ STL headers
#include <ostream>
#include <vector>
#include <memory>

// LLVM headers
#include <llvm/ADT/DenseMap.h>
#include <llvm/Pass.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/IR/BasicBlock.h>

// libNTS
#include <libNTS/nts.hpp>

struct llvm2nts_options
{
	unsigned int thread_poll_size;
};

std::unique_ptr<nts::Nts> llvm_to_nts (
		const llvm::Module & llvm_module,
		const llvm2nts_options * opts = nullptr );

std::unique_ptr<nts::Nts> llvm_file_to_nts (
		const std::string filename,
		const llvm2nts_options * opts = nullptr );

#endif // _LLVM2NTS_HPP_
