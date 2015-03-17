/**
 * @file   Call.hpp
 * @brief  'Call' transition rule
 * @date   8. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#ifndef NTS_CALL_HPP_
#define NTS_CALL_HPP_

#include <initializer_list>
#include <vector>

#include "TransitionRule.hpp"
#include "NTS.hpp"
#include "Variable.hpp"

namespace NTS
{
	class BasicNts; // Forward declaration

	class Call final : public TransitionRule
	{
		public:
			Call ( const BasicNts            * nts,
				   const std::vector
				   	   < const Variable * > &  outs,
				   const std::vector
				   	   < const IPrint   * > &  ins );

			~Call() {;}

			virtual void print ( std::ostream &o ) const override;

		private:
			const BasicNts                 * m_nts;
			std::vector < const IPrint   * > m_ins;
			std::vector < const Variable * > m_outs;

	};

}

#endif /* NTS_CALL_HPP_ */
