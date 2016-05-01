/*
	ppmatrix.h
	
	Representation of a (P)artially (P)artitioned matrix.
*/

#pragma once

#include <vector>


class ppmatrix {
	
public:
	// The matrix being partitioned.
	const matrix &m;
	
	// The current status of all rows and columns.
	std::vector<status> status[2];
	
	// The current size of the partition, and the maximal
	// size of a partition.
	size_t partition_size[2] = {0, 0};
	const size_t max_partition_size;
	
	ppmatrix(matrix &_m, double epsilon)
		: m(_m) {
		row.assign(m.R, status::unassigned);
		col.assign(m.C, status::unassigned);
		max_partition_size = (size_t)floor((1.0 + epsilon) * m.NZ / 2.0);
	}
	
	// Returns whether the current assignment may lead to
	// a valid partition.
	bool valid();
	
	// Gives a lower bound on the volume of all partitions
	// that can be extended from the current, partial partition.
	size_t lower_bound();

private:
	const size_t 
};

