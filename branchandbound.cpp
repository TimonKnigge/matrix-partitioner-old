#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

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
void recurse(size_t &next_rc, const std::vector<row_or_col> &rows_columns, std::stack<operation> &st, const ppmatrix &ppm) {
	
	row_or_col rc = rows_columns[next_rc];
	
	if (ppm.can_assign(rc, status::cut))
		recurse_single(rc, st, ppm, status::cut);
	if (ppm.can_assign(rc, status::red))
		recurse_single(rc, st, ppm, status::red);
	if (ppm.can_assign(rc, status::blue))
		recurse_single(rc, st, ppm, status::blue);
	
	++next_rc;
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
			return m.adj[l.rowcol][l.index] > m.adj[r.rowcol][r.index];	
	});
	
	// Prepare to maintain an optimal solution
	int optimal_value = -1;
	row.assign(m.R, status::unassigned);
	col.assign(m.C, status::unassigned);
	
	// Simulate recursion in the branch-and-bound tree
	std::stack<operation> call_stack;
	recurse(next_rc, rows_columns, call_stack, partial_partition);
	while (call_stack.size() > 0) {
		
		if (call_stack.top().type == operation_type::descend) {
			
			// TODO: Calculate/update the lowerbound
			
			call_stack.pop();
			if (next_rc < rows_columns.size()) {

				if (partial_partition.valid() && (partial_partition.lower_bound() < optimal_value || optimal_value == -1))
					recurse(next_rc, rows_columns, call_stack, partial_partition);
			} else {
				
				if (partial_partition.valid() && (partial_partition.lower_bound() < optimal_value || optimal_value == -1)) {
					optimal_value = partial_partition.lower_bound();
					std::copy(
						partial_partition.stat[ROW].begin(),
						partial_partition.stat[ROW].end(),
						row.begin());
					std::copy(
						partial_partition.stat[COL].begin(),
						partial_partition.stat[COL].end(),
						col.begin());
				}

				++next_rc;
			}
		} else {
			
			// TODO: Calculate/update the lowerbound
			
			call_stack.pop();
			--next_rc;
		}
	}
	
	return optimal_value;
}
