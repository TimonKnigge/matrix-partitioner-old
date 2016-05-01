#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "matrix.h"
#include "abstract_partitioner.h"
#include "ppmatrix.h"


void ppmatrix::set_epsilon(double epsilon) {
	max_partition_size = (size_t)floor((1.0 + epsilon) * m.NZ / 2.0);
}

bool ppmatrix::can_assign(row_or_col rc, status newstat) const {
	
	status curstat = stat[rc.rowcol][rc.index];
	switch (newstat) {
		case status::red:
			return (curstat == status::unassigned || curstat == status::partialred);
		case status::blue:
			return (curstat == status::unassigned || curstat == status::partialblue);
		case status::cut:
			return true;
		default:
			return false;
	}
}

bool ppmatrix::valid() const {
	return partition_size[ROW] <= max_partition_size
		&& partition_size[COL] <= max_partition_size;
}

int ppmatrix::lower_bound() const {
	return (int)(cut + implicitely_cut);
}
