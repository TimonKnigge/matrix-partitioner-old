/*
	resettable_array.h
	
	An array that can be reset in O(1) time.
*/

#pragma once

#include <array>

// Type T, size N
template<class T, size_t N>
class resettable_array {

public:
	// Default value to be returned
	T def;
	// Current value of each position
	std::array<T, N> value;
	
	// Current 'time'
	size_t current_time = 0;
	// Time of last set for each value
	std::array<size_t, N> timestamp;
	
	resettable_array(T init) {
		def = init;
		for (size_t i = 0; i < N; ++i) {
			value[i] = def;
			timestamp[i] = 0;
		}
	}
	
	T get(size_t i); // does not do bounds checking!
	void set(size_t i, T val);
	void reset_all();
	void reset_all(T val);
};

