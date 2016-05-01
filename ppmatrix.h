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
	std::vector<status> stat[2];
	
	// The current size of the partition, and the maximal
	// size of a partition.
	size_t partition_size[2] = {0, 0};
	size_t max_partition_size;
	
	ppmatrix(const matrix &_m)
		: m(_m) {
		stat[ROW].assign(m.R, status::unassigned);
		stat[COL].assign(m.C, status::unassigned);
	}
	
	// Returns whether the current assignment may lead to
	// a valid partition.
	bool valid();
	
	// Gives a lower bound on the volume of all partitions
	// that can be extended from the current, partial partition.
	int lower_bound();

private:
	const size_t tmp = 0;
};

