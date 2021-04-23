#pragma once

#include <corecrt_math.h>

typedef float_t float32_t;
typedef double_t float64_t;

class object_t
{
public:
	object_t() {};
	virtual ~object_t() {}; // using pure virtual
};

