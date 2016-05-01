/*
	branchandbound.h
	
	Describes the branch and bound partitioner
*/

#pragma once

#include <vector>

enum class operation_type { descend, undo };
struct operation {
	operation_type type;
	size_t rowcol;
	size_t index;
	status value;
};

class branchandbound : public abstract_partitioner {
	
public:
	branchandbound(const matrix &_m)
		: abstract_partitioner(_m),
		  partial_partition(_m) { }
	
	// see abstract_partitioner.h
	int partition(double epsilon, std::vector<status> &row, std::vector<status> &col);
	
private:
	ppmatrix partial_partition;
	
};
