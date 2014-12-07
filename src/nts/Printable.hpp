#ifndef _NTS_PRINTABLE_H
#define _NTS_PRINTABLE_H

#include <ostream>

namespace NTS
{
	class Printable
	{
		public:

			// Priority list
			enum Prio
			{
				PR_AlwaysBrackets,

				// logical connectors
				PR_Equiv, // lowest priority
				PR_Imply,
				PR_Or,
				PR_And,
				PR_Not,

				// arithmetical connectors
				PR_Add,
				PR_Div,
				PR_Mod,
				PR_Mul,

				// Highest priority. Can not have children.
				// => no brackets around
				PR_Leaf
			};

		protected:
			const Prio m_prio;

			Printable(Prio prio);

		public:
			std::ostream & wrapped_print(std::ostream &o, enum Prio prio) const;
			virtual void print(std::ostream &o) const = 0;

	};
};

#endif // _NTS_PRINTABLE_H
