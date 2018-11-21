#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <stdexcept>

#define assert(condition, message) {\
	if (!(condition)) {\
		throw std::runtime_error(message);\
	}\
}\

#endif