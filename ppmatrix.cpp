#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "matrix.h"
#include "ppmatrix.h"


void ppmatrix::set_epsilon(double epsilon) {
	max_partition_size = (size_t)floor((1.0 + epsilon) * m.NZ / 2.0);
}

bool ppmatrix::valid() {
	return partition_size[ROW] <= max_partition_size
		&& partition_size[COL] <= max_partition_size;
}

int ppmatrix::lower_bound() {
	return (int)(cut + implicitely_cut);
}
