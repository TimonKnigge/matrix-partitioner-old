#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "matrix.h"
#include "abstract_partitioner.h"
#include "branchandbound.h"

void recurse(size_t &next_rc, std::vector<row_or_col> &rows_columns, std::stack<operation> &st) {
	
	for (auto stat : {status::cut, status::blue, status::red}) {
		st.push({
			operation_type::undo,
			rows_columns[next_rc].rowcol,
			rows_columns[next_rc].index,
			stat});
		st.push({
			operation_type::descend,
			rows_columns[next_rc].rowcol,
			rows_columns[next_rc].index,
			stat});
	}
	
	++next_rc;
}

bool branchandbound::partition(double epsilon, std::vector<status> &row, std::vector<status> &col) {
	
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
	
	// Simulate recursion in the branch-and-bound tree
	std::stack<operation> call_stack;
	recurse(next_rc, rows_columns, call_stack);
	while (call_stack.size() > 0) {
		
		if (call_stack.top().type == operation_type::descend) {
			
			// TODO: Calculate/update the lowerbound
			
			call_stack.pop();
			if (next_rc < rows_columns.size()) {

				// TODO: Decide if we should continue branching

				recurse(next_rc, rows_columns, call_stack);
			} else {
				
				// TODO: Store the solution if optimal

				++next_rc;
			}
		} else {
			
			// TODO: Calculate/update the lowerbound
			
			call_stack.pop();
			--next_rc;
		}
	}
	
	return false;
}
