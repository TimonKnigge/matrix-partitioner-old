#include <array>
#include <limits>

#include "resettable_array.h"


template <class T>
T resettable_array<T>::get(size_t i) {
	return (current_time == timestamp[i] ? value[i] : def);
}

template <class T>
void resettable_array<T>::set(size_t i, T val) {
	value[i] = val;
	timestamp[i] = current_time;
}

template <class T>
void resettable_array<T>::reset_all() {
	++current_time;
	// Prevent overflow (todo: investigate necessity)
	if (current_time == std::numeric_limits<size_t>::max()) {
		current_time = 0;
		for (size_t i = 0; i < value.size(); ++i) {
			value[i] = def;
			timestamp[i] = 0;
		}
	}
}

template<class T>
void resettable_array<T>::reset_all(T val) {
	def = val;
	reset_all();
}
