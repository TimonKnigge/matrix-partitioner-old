/*
	constants.h
	
	Define a series of constants that determine which
	bounds are turned on/off (i.e., used in the branch
	and bound algorithm.
	
	While it is usually preferable to define constants
	using constexpr and not using #define's, doing it
	this way allows us to hide large datastructures
	maintained using the branch and bound algorithm,
	at compile time.
*/

#pragma once

#include <cassert>

	#define PACKING_BOUND_1
//	#define PACKING_BOUND_2
	#define FLOW_BOUND_1

inline void assert_correctness_of_constants() {

	// Only one packing bound can be ran at once
#if defined(PACKING_BOUND_1) && defined(PACKING_BOUND_2)
#error Can only use one packing bound at a time.
#endif

}
