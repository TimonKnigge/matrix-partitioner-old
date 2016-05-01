/*
	abstract_partitioner.h
	
	Describes the interface of a partitioner
*/

#pragma once

#include <vector>

enum class status {
	unassigned = 2, red = 0, blue = 1, cut = 4, 
	partialblue = 5, partialred = 6, implicitcut = 7
};

inline status to_partial(status st) {
	return (st == status::red ? status::partialred : status::partialblue);
}
inline status from_partial(status st) {
	return (st == status::partialred ? status::red : status::blue);
}

class abstract_partitioner {

public:
	// Constructs a matrix partitioner around the given matrix.
	abstract_partitioner(matrix &_m)
		: m(_m) { }
	
	// Partitions a matrix with an (1+epsilon) balance. Returns -1 
	// if the matrix could not be partitioned, and the optimal volume
	// otherwise. If the matrix is successfully partitioned, the row 
	// and col vectors contain an assignment of the rows and columns
	// (all elements will have the form unassigned, red or blue).
	virtual int partition(double epsilon, std::vector<status> &row, std::vector<status> &col) = 0;
	
protected:
	matrix &m;
};

