#include <string>
#include "util.hpp"

using namespace nts;
using std::string;
using std::to_string;

State *  new_state ( unsigned int bb_id, unsigned int inst_id )
{
	return new State (
			string    ( "st_"   ) +
			to_string ( bb_id   ) +
			string    ( "_"     ) +
			to_string ( inst_id )
	);

}

