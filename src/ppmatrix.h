/*
	ppmatrix.h
	
	Representation of a (P)artially (P)artitioned matrix.
*/

#pragma once

#include <vector>

#include "constants.h"
#include "vertex_cut_graph.h"

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

#ifdef PACKING_BOUND_1
	// free_in_partial[ROW/COL][red/blue][cnt] for
	// 0<=cnt<=m.Cmax gives the amount of partially
	// colored rows/columns having cnt free nonzeros.
	std::vector<size_t> free_in_partial[2][2];
	// total_free_in_partial[ROW/COL][red/blue] gives
	// the number of free nonzeros in rows/columns
	// partially assigned to the given column
	size_t total_free_in_partial[2][2];
#endif

#ifdef FLOW_BOUND_1
	vertex_cut_graph G;
#endif
	
	ppmatrix(const matrix &_m)
		: m(_m), G(m.R + m.C) {
		stat[ROW].assign(m.R, status::unassigned);
		stat[COL].assign(m.C, status::unassigned);
		for (size_t c = 0; c < 2; ++c) {
			color_count[ROW][c].assign(m.R, 0);
			color_count[COL][c].assign(m.C, 0);
#ifdef PACKING_BOUND_1
			for (size_t rc = 0; rc < 2; ++rc) {
				free_in_partial[rc][c].assign(m.Cmax+1, 0);
				total_free_in_partial[rc][c] = 0;
			}
#endif

		}
#ifdef FLOW_BOUND_1
		for (size_t r = 0; r < m.R; ++r)
			for (auto &&e : m.rows[r]) {
				G.add_edge(r, m.R + e->c, 1);
				G.add_edge(m.R + e->c, r, 1);
			}
#endif
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
	int lower_bound();
	
	// Sets the epsilon for the partition size (1+epsilon) partitioning
	void set_epsilon(double epsilon);

	// Overload I\O
	friend std::istream &operator>>(std::istream &stream,       ppmatrix &ppm);
	friend std::ostream &operator<<(std::ostream &stream, const ppmatrix &ppm);

private:
	size_t cut = 0, implicitely_cut = 0;

#ifdef PACKING_BOUND_1
	// A lower bound on the size of the current partition,
	// based on the number of free nonzeros in partially
	// partitions rows/columns.
	size_t packing_lower_bound = 0;
	bool packing_lower_bound_valid = false;
	void recalculate_packing_lower_bound();
#endif
};

