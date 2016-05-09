/*
	abstract_partitioner.h
	
	Describes the interface of a partitioner
*/

#pragma once

#include <vector>

enum class status {
	unassigned = 2, red = 0, blue = 1, cut = 3, 
	partialblue = 4, partialred = 5, implicitcut = 6
};

inline status to_partial(status st) {
	return (st == status::red ? status::partialred : status::partialblue);
}
inline status from_partial(status st) {
	return (st == status::partialred ? status::red : status::blue);
}
inline status color_swap(status st) {
	if (st == status::partialred ) return status::partialblue;
	if (st == status::partialblue) return status::partialred;
	if (st == status::red ) return status::blue;
	if (st == status::blue) return status::red;
	return st;
}
inline bool is_partial(status st) {
	return (st == status::partialred || st == status::partialblue);
}

class abstract_partitioner {

public:
	// Constructs a matrix partitioner around the given matrix.
	abstract_partitioner(const matrix &_m)
		: m(_m) { }
	
	// Partitions a matrix with an (1+epsilon) balance. Returns -1 
	// if the matrix could not be partitioned, and the optimal volume
	// otherwise. If the matrix is successfully partitioned, the row 
	// and col vectors contain an assignment of the rows and columns
	// (all elements will have the form unassigned, red or blue).
	virtual int partition(double epsilon, std::vector<status> &row, std::vector<status> &col) = 0;
	
protected:
	const matrix &m;
};

