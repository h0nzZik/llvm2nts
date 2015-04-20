#ifndef SUGAR_HPP_
#define SUGAR_HPP_
#pragma once

#include <memory>

//#include <named-operator/include/base/named_operator.hpp>
#include <libNTS/logic.hpp>

namespace sugar
{

#if 0
namespace op
{
	std::unique_ptr < nts::Relation > eq (
			std::unique_ptr < nts::Term > &,
			std::unique_ptr < nts::Term > & 
	);
};

auto eq = base::make_named_operator ( op::eq );
#endif

std::unique_ptr < nts::Relation > operator== (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

std::unique_ptr < nts::Relation > operator> (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

std::unique_ptr < nts::Relation > operator>= (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

std::unique_ptr < nts::Relation > operator< (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

std::unique_ptr < nts::Relation > operator<= (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

std::unique_ptr < nts::Relation > operator!= (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

// Compare to zero ( is negative )
std::unique_ptr < nts::Relation > operator< (
		std::unique_ptr < nts::Term > &,
		int );


nts::FormulaBop & operator== ( nts::Formula & f1, nts::Formula &f2 );

nts::FormulaBop & operator&& ( nts::Formula & f1, nts::Formula & f2 );

std::unique_ptr < nts::FormulaBop > operator&& (
		std::unique_ptr < nts::Formula > &&,
		std::unique_ptr < nts::Formula > && );

std::unique_ptr < nts::FormulaBop > operator== (
		std::unique_ptr < nts::Formula > &&,
		std::unique_ptr < nts::Formula > && );



std::unique_ptr < nts::FormulaBop > equally_negative (
		std::unique_ptr < nts::Term > &,
		std::unique_ptr < nts::Term > & );

};

#endif // SUGAR_HPP_
