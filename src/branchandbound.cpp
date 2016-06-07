#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <limits>

#include "constants.h"
#include "matrix.h"
#include "abstract_partitioner.h"
#include "ppmatrix.h"
#include "branchandbound.h"

void recurse_single(row_or_col rc, std::stack<operation> &st, const ppmatrix &ppm, status stat) {
	
	st.push({
		operation_type::undo,
		rc.rowcol, rc.index,
		ppm.stat[rc.rowcol][rc.index]});
	st.push({
		operation_type::descend,
		rc.rowcol, rc.index,
		stat});
	
}
void recurse(size_t &next_rc, const std::vector<row_or_col> &rows_columns, std::stack<operation> &st, ppmatrix &ppm) {
	
	row_or_col rc = rows_columns[next_rc];
	
	if (ppm.m.adj[rc.rowcol][rc.index].size() == 0) {
		recurse_single(rc, st, ppm, status::red);
	}
	
	std::pair<status, int> br[3];
	br[0].first = status::red;
	br[1].first = status::blue;
	br[2].first = status::cut;
	
	status cur = ppm.stat[rc.rowcol][rc.index];
	for (size_t i = 0; i < 3; ++i) {
		if (!ppm.can_assign(rc, br[i].first))
			br[i].second = std::numeric_limits<int>::max();
		else {
			ppm.assign(rc, br[i].first);
			if (ppm.valid())
				br[i].second = ppm.lower_bound();
			else	br[i].second = std::numeric_limits<int>::max();
			ppm.undo(rc, cur);
		}
	}
	
	// (randomized) sorting network for 3 values
	if (br[0].second > br[1].second) swap(br[0], br[1]);
	if (br[0].second == br[1].second && (rand()&1)) swap(br[0], br[1]);

	if (br[1].second > br[2].second) swap(br[1], br[2]);
	if (br[1].second == br[2].second && (rand()&1)) swap(br[1], br[2]);

	if (br[0].second > br[1].second) swap(br[0], br[1]);
	if (br[0].second == br[1].second && (rand()&1)) swap(br[0], br[1]);
	
	for (size_t i = 0; i < 3; ++i) {
		if (br[2-i].second < std::numeric_limits<int>::max())
			recurse_single(rc, st, ppm, br[2-i].first);
	}
}

int branchandbound::partition(double epsilon, std::vector<status> &row, std::vector<status> &col) {
	
	// A list of all rows and columns, sort them in non-ascending order
	// by number of nonzeros.
	size_t next_rc = 0;
	std::vector<row_or_col> rows_columns;
	for (size_t r = 0; r < m.R; ++r)
		rows_columns.push_back({ROW, r});
	for (size_t c = 0; c < m.C; ++c)
		rows_columns.push_back({COL, c});
	sort(rows_columns.begin(), rows_columns.end(),
		[this](const row_or_col &l, const row_or_col &r) {
			return m.adj[l.rowcol][l.index].size() > m.adj[r.rowcol][r.index].size();	
	});
	
	// Prepare to maintain an optimal solution
	int optimal_value = std::min(m.R, m.C) + 2;
	row.assign(m.R, status::unassigned);
	col.assign(m.C, status::unassigned);
	
	// Check validity
	partial_partition.set_epsilon(epsilon);
	if (2 * partial_partition.max_partition_size < m.NZ) {
		std::cerr << "Current values of epsilon and NZ do not allow a partition." << std::endl;
		return -1;
	}

	// Simulate recursion in the branch-and-bound tree
	std::stack<operation> call_stack;
	recurse(next_rc, rows_columns, call_stack, partial_partition);
	int a = 0;
	while (call_stack.size() > 0) {
		a++;
//		std::cerr << a << '\t' << partial_partition.lower_bound() << '\t' << partial_partition.lower_bound(true) << std::endl;
		if (call_stack.top().type == operation_type::descend) {
			
			partial_partition.assign(
				{call_stack.top().rowcol,
				 call_stack.top().index},
				call_stack.top().value);
			
			call_stack.pop();
			++next_rc;

			if (next_rc < rows_columns.size()) {

				if (partial_partition.valid() && (partial_partition.lower_bound() < optimal_value)) {
					recurse(next_rc, rows_columns, call_stack, partial_partition);
				}
			} else {
				if (partial_partition.valid()) ++leaves;

				if (partial_partition.valid() && (partial_partition.lower_bound() < optimal_value)) {
					optimal_value = partial_partition.lower_bound();
				std::cerr << optimal_value << ' ' << a << ' ' << std::endl;
					
					std::copy(
						partial_partition.stat[ROW].begin(),
						partial_partition.stat[ROW].end(),
						row.begin());
					std::copy(
						partial_partition.stat[COL].begin(),
						partial_partition.stat[COL].end(),
						col.begin());
				}
				
			}
		} else {
			
			partial_partition.undo(
				{call_stack.top().rowcol,
				 call_stack.top().index},
				call_stack.top().value);
			
			call_stack.pop();
			--next_rc;
		}
	}
	
	return optimal_value;
}

void print_colored(const matrix &m, std::vector<status> &r, std::vector<status> &c) {
	std::string RED  = "\033[31m";
	std::string BLUE = "\033[34m";
	std::string NONE = "\033[39m";

	std::cerr << "  ";
	for (size_t i = 0; i < m.C; ++i) {
		if (c[i] == status::red) std::cerr << RED  << 'R';
		if (c[i] == status::blue)std::cerr << BLUE << 'B';
		if (c[i] == status::cut) std::cerr << NONE << 'C';
	}
	std::cerr << std::endl;
	for (size_t i = 0; i < m.R; ++i) {
		if (r[i] == status::red) std::cerr << RED  << 'R';
		if (r[i] == status::blue)std::cerr << BLUE << 'B';
		if (r[i] == status::cut) std::cerr << NONE << 'C';
		std::cerr << ' ';
		for (size_t j = 0, k = 0; k < m.C; ++k) {
			if (j < m.rows[i].size() && m.rows[i][j]->c == k) {
				if (r[i] == status::red || c[k] == status::red)
					std::cerr << RED << '#';
				else
				if (r[i] == status::blue || c[k] == status::blue)
					std::cerr << BLUE << '#';
				else    std::cerr << NONE << '#';
				++j;
			} else std::cerr << NONE << '.';
		}
		std::cerr << std::endl;
	}
}
