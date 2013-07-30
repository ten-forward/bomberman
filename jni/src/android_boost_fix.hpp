#ifndef ANDROID_BOOST_FIX_HPP
#define ANDROID_BOOST_FIX_HPP

#ifdef _

// The android NDK's lack of exception support means that
// certain classes are excluded from standard library.
// Oddly enough this set of classes excluded is quite
// arbitrary. This file is used to silence compiler
// errors related to the absence of these classes.

// That said, you can still catch exceptions in C++,
// Java won't do anything friendly if you let the
// exception go to the VM.

namespace std
{
	class out_of_range : public std::exception
	{
	public:
		out_of_range(const char*);
	};

	class runtime_error : public std::exception
	{
	public:
		runtime_error(const char*);
	};
}

#endif // ANDROID

#endif // ANDROID_BOOST_FIX_HPP
