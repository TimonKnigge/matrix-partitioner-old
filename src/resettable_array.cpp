#include <array>
#include <limits>

#include "resettable_array.h"


template <class T, size_t N>
T resettable_array<T, N>::get(size_t i) {
	return (current_time == timestamp[i] ? value[i] : def);
}

template <class T, size_t N>
void resettable_array<T, N>::set(size_t i, T val) {
	value[i] = val;
	timestamp[i] = current_time;
}

template <class T, size_t N>
void resettable_array<T, N>::reset_all() {
	++current_time;
	// Prevent overflow (todo: investigate necessity)
	if (current_time == std::numeric_limits<size_t>::max()) {
		current_time = 0;
		for (size_t i = 0; i < N; ++i) {
			value[i] = def;
			timestamp[i] = 0;
		}
	}
}

template<class T, size_t N>
void resettable_array<T, N>::reset_all(T val) {
	def = val;
	reset_all();
}
