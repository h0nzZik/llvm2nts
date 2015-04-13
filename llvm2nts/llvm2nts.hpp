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

std::unique_ptr<nts::Nts> llvm_to_nts ( const llvm::Module & llvm_module );

#endif // _LLVM2NTS_HPP_
