/*
	branchandbound.h
	
	Describes the branch and bound partitioner
*/

#pragma once

#include <vector>

#include "branchandbound.h"
#include "matrix.h"

class branchandbound : public abstract_partitioner {
	
public:
	branchandbound(matrix &_m)
		: abstract_partitioner(_m) { }
	
	// see abstract_partitioner.h
	bool partition(double epsilon, vector<status> &row, vector<status> &col);
	
//private:
	
};
