/*
 * @file   Constants.cpp
 * @date   1. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */
#include <stdexcept>
#include <sstream>
#include <llvm2nts/instructions/Constants.hpp>

using namespace NTS;

Constants::Constants() :
		bool_true  ( "1" ),
		bool_false ( "0" )
{
	;
}

Constants::~Constants()
{
	for (BitsizeGroup &g : m_bs)
	{
		if (g.unsigned_bound)
		{
			delete g.unsigned_bound;
			g.unsigned_bound = nullptr;
		}

		if (g.signed_bound)
		{
			delete g.signed_bound;
			g.signed_bound = nullptr;
		}
	}
}

const Constants::BitsizeGroup & Constants::get_bitsize_group ( unsigned int bitsz )
{
	// Only bit size of 1 to 63 bits is supported
	// TODO: add support for greater scales (eg using APInt)
	if (bitsz < 1 || bitsz >= 8 * sizeof(unsigned long long int))
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


