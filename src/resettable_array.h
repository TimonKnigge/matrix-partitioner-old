/*
	resettable_array.h
	
	An array that can be reset in O(1) time.
*/

#pragma once

#include <vector>

// Type T
template<class T>
class resettable_array {
	
public:
	// Default value to be returned
	T def;
	// Current value of each position
	std::vector<T> value;
	
	// Current 'time'
	size_t current_time = 0;
	// Time of last set for each value
	std::vector<size_t> timestamp;
	
	resettable_array(size_t N, T init) {
		value.assign(N, init);
		timestamp.assign(N, 0);
		def = init;
	}
	
	T get(size_t i); // does not do bounds checking!
	void set(size_t i, T val);
	void reset_all();
	void reset_all(T val);
};

