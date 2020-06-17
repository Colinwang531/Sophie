#include "Number/Number.h"

namespace base
{
	namespace number
	{
		OddNumber::OddNumber() : number{ 1 }
		{}

		OddNumber::~OddNumber()
		{}

		const long long OddNumber::getOddNumber()
		{
			const long long n{ number };
			number += 2;
			return n;
		}
	}//namespace number
}//namespace base
