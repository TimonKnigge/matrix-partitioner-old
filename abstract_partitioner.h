/*
	abstract_partitioner.h
	
	Describes the interface of a partitioner
*/

#pragma once

#include <vector>

#include "matrix.h"

enum class status {
	unassigned, red, blue, cut, 
	partialblue, partialred, implicitcut
};

status to_partial(status st) {
	return (st == status.red ? status.partialred : status.partialblue);
}
status from_partial(status st) {
	return (st == status.partialred ? status.red : status.blue);
}

class abstract_partitioner {

public:
	// Constructs a matrix partitioner around the given matrix.
	abstract_partitioner(matrix &_m)
		: m(_m) { }
	
	// Partitions a matrix with an (1+epsilon) balance. Returns false 
	// if the matrix could not be partitioned, and true otherwise. If
	// the matrix is successfully partitioned, the row and col vectors
	// contain an assignment of the rows and columns (all elements will
	// have the form unassigned, red or blue).
	virtual bool partition(double epsilon, vector<status> &row, vector<status> &col) = 0;
	
protected:
	matrix &m
};

