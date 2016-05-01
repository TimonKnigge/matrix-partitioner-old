/*
	matrix.h
	
	Representation of a matrix to be partitioned.
*/

#pragma once

#include <iostream>
#include <vector>
#include <memory>

constexpr size_t ROW = 0, COL = 1;
inline size_t invert_rowcol(size_t rc) {
	return 1 - rc;
}

// Represents a non-zero in the matrix
struct entry {
	// The row and column in which the entry is positioned
	size_t pos[2], &r = pos[ROW], &c = pos[COL];
	// The indices in the row and column list of the matrix
	size_t index[2], &ri = index[ROW], &ci = index[COL];
};

// Represents a row or column
struct row_or_col {
	size_t rowcol;
	size_t index;
};

class matrix {

public:
	// The entries of the matrix, by row and column
	std::vector< std::vector< std::shared_ptr<entry> > > 
		adj[2], &rows = adj[0], &cols = adj[1];
	
	// The number of rows, columns and non-zeros in the matrix
	size_t R, C;
	int NZ = 0;

private:
	// Add a nonzero entry in position (r, c) with 0<=r<R and
	// 0<=c<C
	void add_entry(size_t r, size_t c);
	
	// Fixes the internal adjacency lists
	void sort_adjacency_lists();
	
	// Overload I\O
	friend std::istream &operator>>(std::istream &stream,       matrix &m);
	friend std::ostream &operator<<(std::ostream &stream, const matrix &m);
};



