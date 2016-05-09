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
	
	// Count the number of colors in each row/column:
	// color_count[ROW/COL][red/blue][index]
	std::vector<size_t> color_count[2][2];
	
	ppmatrix(const matrix &_m)
		: m(_m) {
		stat[ROW].assign(m.R, status::unassigned);
		stat[COL].assign(m.C, status::unassigned);
		for (size_t c = 0; c < 2; ++c) {
			color_count[ROW][c].assign(m.R, 0);
			color_count[COL][c].assign(m.C, 0);
		}
	}
	
	// Decides whether the given row/colulmn can get the given
	// status.
	bool can_assign(row_or_col rc, status stat) const;
	
	// Assigns the given row/column the given status.
	void assign(row_or_col rc, status stat);
	
	// Undoes the above assignment, with a hint towards the old status.
	void undo(row_or_col rc, status hint);
	
	// Returns whether the current assignment may lead to
	// a valid partition.
	bool valid() const;
	
	// Gives a lower bound on the volume of all partitions
	// that can be extended from the current, partial partition.
	int lower_bound() const;
	
	// Sets the epsilon for the partition size (1+epsilon) partitioning
	void set_epsilon(double epsilon);

	// Overload I\O
	friend std::istream &operator>>(std::istream &stream,       ppmatrix &ppm);
	friend std::ostream &operator<<(std::ostream &stream, const ppmatrix &ppm);

//private:
	size_t cut = 0, implicitely_cut = 0;
};

