/**
 * @file   NtsDeclaration.hpp
 * @date   17. 3. 2015
 * @author Jan Tusil <jenda.tusil@gmail.com>
 */

#ifndef NTS_NTSDECLARATION_HPP_
#define NTS_NTSDECLARATION_HPP_

namespace NTS
{
	class NtsDeclaration : public NtsRef
	{
		public:
			NtsDeclaration ( const std::string & name) :
				m_name ( name )
			{
				;
			}

			virtual ~NtsDeclaration() {};

			virtual const std::string & get_name () const override
			{
				return m_name;
			}

		private:
			std::string m_name;
	};

} /* namespace NTS */

#endif /* NTS_NTSDECLARATION_HPP_ */
