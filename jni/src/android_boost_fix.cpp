#ifdef _

#include <iostream>
#include "android_boost_fix.hpp"

namespace boost
{
	void throw_exception(std::exception const&) {}
}

namespace std
{
	out_of_range::out_of_range(const char*) 
	{
	}
}

#endif // ANDROID
