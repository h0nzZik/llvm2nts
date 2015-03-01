/*
 * @file   Constants.hpp
 * @date   1. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#ifndef LLVM2NTS_INSTRUCTIONS_CONSTANTS_HPP_
#define LLVM2NTS_INSTRUCTIONS_CONSTANTS_HPP_

#include <vector>
#include <nts/ConcreteArithmetic.hpp>

/**
 * @brief Manages important constants (true, false, boundary values for bitvector types).
 */
class Constants
{
public:
	Constants();
	Constants ( const Constants & other  ) = delete;
	Constants ( const Constants && other ) = delete;

	virtual ~Constants();

	struct BitsizeGroup
	{
		NTS::ConPositiveNumeral * unsigned_bound;
		NTS::ConPositiveNumeral * signed_bound;
	};


	const BitsizeGroup & get_bitsize_group ( unsigned int bitsz );

	const NTS::ConPositiveNumeral bool_true;
	const NTS::ConPositiveNumeral bool_false;

private:

	std::vector<BitsizeGroup> m_bs;
};

#endif /* LLVM2NTS_INSTRUCTIONS_CONSTANTS_HPP_ */
