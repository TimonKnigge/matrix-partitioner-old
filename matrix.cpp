#include <iostream>
#include <memory>
#include <algorithm>

#include "matrix.h"


std::istream &operator>>(std::istream &stream, matrix &m) {
	
	
	
	
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const matrix &m) {
	
	for (size_t r = 0; r < m.R; ++r) {
		size_t j = 0;
		for (size_t i = 0; i < m.C; ++i) {
			if (j < m.rows[r].size() && m.rows[r][j]->c == i) {
				stream << '#';
				++j;
			} else	stream << '.';
		}
		stream << '\n';
	}
	
	return stream;
}

void matrix::add_entry(size_t r, size_t c) {
	entry e;
	e.r = r;
	e.c = c;
	e.ri = rows[r].size();
	e.ci = cols[c].size();
	
	rows[r].push_back(std::make_shared<entry>(e));
	cols[c].push_back(std::make_shared<entry>(e));
}

void matrix::sort_adjacency_lists() {
	
	// Sort the entries in each row by their columns
	for (size_t r = 0; r < R; ++r) {
		std::sort(rows[r].begin(), rows[r].end(),
			[](const std::shared_ptr<entry> &l, const std::shared_ptr<entry> &r) {
			return l->c < r->c;
		});
		for (size_t i = 0; i < rows[r].size(); ++i)
			rows[r][i]->ri = i;
	}
	
	// Sort the entries in each column by their rows
	for (size_t c = 0; c < C; ++c) {
		std::sort(cols[c].begin(), cols[c].end(),
			[](const std::shared_ptr<entry> &l, const std::shared_ptr<entry> &r) {
			return l->r < r->r;
		});
		for (size_t i = 0; i < cols[c].size(); ++i)
			cols[c][i]->ci = i;
	}
}
