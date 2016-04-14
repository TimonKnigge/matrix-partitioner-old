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
	
	
};

