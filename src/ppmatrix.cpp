#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "constants.h"
#include "matrix.h"
#include "abstract_partitioner.h"
#include "ppmatrix.h"


void ppmatrix::set_epsilon(double epsilon) {
	max_partition_size = (size_t)floor((1.0 + epsilon) * m.NZ / 2.0);
}

bool ppmatrix::can_assign(row_or_col rc, status newstat) const {
	
	status curstat = stat[rc.rowcol][rc.index];
	switch (newstat) {
		case status::red:
			return (curstat == status::unassigned || curstat == status::partialred);
		case status::blue:
			return (curstat == status::unassigned || curstat == status::partialblue);
		case status::cut:
			return true;
		default:
			return false;
	}
}

bool ppmatrix::valid() const {
	return partition_size[ROW] <= max_partition_size
		&& partition_size[COL] <= max_partition_size;
}

int ppmatrix::lower_bound() const {
	return (int)(cut + implicitely_cut);
}

void ppmatrix::assign(row_or_col rc, status newstat) {
	
	status oldstat = stat[rc.rowcol][rc.index];

#ifdef PACKING_BOUND_1
	for (status s : {status::partialred, status::partialblue}) {
		if (oldstat != s) continue;

		int color = static_cast<int>(from_partial(s));
		size_t free = m.adj[rc.rowcol][rc.index].size()
			- color_count[rc.rowcol][color][rc.index];
		
		total_free_in_partial[rc.rowcol][color] -= free;
		--free_in_partial[rc.rowcol][color][free];
		
	}
#endif
	
	switch (newstat) {
		case status::cut:
		{
			if (oldstat == status::implicitcut)
				--implicitely_cut;
			stat[rc.rowcol][rc.index] = newstat;
			++cut;
			break;
		}
		case status::red:
		case status::blue:
		{
			int color = static_cast<int>(newstat);
			if (oldstat == status::implicitcut)
				--implicitely_cut;
			stat[rc.rowcol][rc.index] = newstat;
			for (auto e : m.adj[rc.rowcol][rc.index]) {
				
				size_t irc = invert_rowcol(rc.rowcol);
				size_t iindex = e->pos[irc];
				
				status &other = stat[irc][iindex];
				if (other == newstat) continue;

#ifdef PACKING_BOUND_1
				size_t free = m.adj[irc][iindex].size()
					- color_count[irc][0][iindex]
					- color_count[irc][1][iindex];
				if (is_partial(other)) {
					int icolor = static_cast<int>(from_partial(other));
					total_free_in_partial[irc][icolor] -= free;
					--free_in_partial[irc][icolor][free];
				}
				--free;
#endif
				
				++color_count[rc.rowcol][color][rc.index];
				++color_count[invert_rowcol(rc.rowcol)][color][e->pos[invert_rowcol(rc.rowcol)]];
				++partition_size[color];
				
#ifdef PACKING_BOUND_1
				if (is_partial(other)) {
					int icolor = static_cast<int>(from_partial(other));
					if (icolor == color) {
						total_free_in_partial[irc][icolor] += free;
						++free_in_partial[irc][icolor][free];
					}
				}
#endif
				if (other == status::unassigned) {
					other = to_partial(newstat);
#ifdef PACKING_BOUND_1
					total_free_in_partial[irc][color] += free;
					++free_in_partial[irc][color][free];
#endif
				}
				if (other == to_partial(color_swap(newstat))) {
					other = status::implicitcut;
					++implicitely_cut;
				}
			}
			break;
		}
		default:break;
	}
}

void ppmatrix::undo(row_or_col rc, status oldstat) {
	
	status newstat = stat[rc.rowcol][rc.index];

	switch (newstat) {
		case status::cut:
		{
			if (oldstat == status::implicitcut)
				++implicitely_cut;
			stat[rc.rowcol][rc.index] = oldstat;
			--cut;
			break;
		}
		case status::red:
		case status::blue:
		{
			int color = static_cast<int>(newstat);
			if (oldstat == status::implicitcut)
				++implicitely_cut;
			stat[rc.rowcol][rc.index] = oldstat;
			for (auto e : m.adj[rc.rowcol][rc.index]) {
				
				size_t irc = invert_rowcol(rc.rowcol);
				size_t iindex = e->pos[irc];
				
				status &other = stat[irc][iindex];
				if (other == newstat) continue;

#ifdef PACKING_BOUND_1
				size_t free = m.adj[irc][iindex].size()
					- color_count[irc][0][iindex]
					- color_count[irc][1][iindex];
				if (is_partial(other)) {
					int icolor = static_cast<int>(from_partial(other));
					total_free_in_partial[irc][icolor] -= free;
					--free_in_partial[irc][icolor][free];
				}
				++free;
#endif
				
				--color_count[rc.rowcol][color][rc.index];
				--color_count[invert_rowcol(rc.rowcol)][color][e->pos[invert_rowcol(rc.rowcol)]];
				--partition_size[color];
				
				if (other == to_partial(newstat) && color_count[invert_rowcol(rc.rowcol)][color][e->pos[invert_rowcol(rc.rowcol)]] == 0)
					other = status::unassigned;
				if (other == status::implicitcut && color_count[invert_rowcol(rc.rowcol)][color][e->pos[invert_rowcol(rc.rowcol)]] == 0) {
					other = to_partial(color_swap(newstat));
					--implicitely_cut;
				}
#ifdef PACKING_BOUND_1
				if (is_partial(other)) {
					int icolor = static_cast<int>(from_partial(other));
					total_free_in_partial[irc][icolor] += free;
					++free_in_partial[irc][icolor][free];
				}
#endif
			}
			break;
		}
		default:break;
	}

#ifdef PACKING_BOUND_1
	for (status s : {status::partialred, status::partialblue}) {
		if (oldstat != s) continue;

		int color = static_cast<int>(from_partial(s));
		size_t free = m.adj[rc.rowcol][rc.index].size()
			- color_count[rc.rowcol][color][rc.index];
		
		total_free_in_partial[rc.rowcol][color] += free;
		++free_in_partial[rc.rowcol][color][free];
	}
#endif
}

std::ostream &operator<<(std::ostream &stream, const ppmatrix &ppm) {
	std::string RED  = "\033[31m";
	std::string BLUE = "\033[34m";
	std::string NONE = "\033[39m";
	
	stream << " ";
	for (size_t c = 0; c < ppm.m.C; ++c) {
		if (ppm.stat[COL][c] == status::red)
			stream << RED << "R";
		else if (ppm.stat[COL][c] == status::blue)
			stream << BLUE << "B";
		else if (ppm.stat[COL][c] == status::cut)
			stream << NONE << "C";
		else	stream << NONE << static_cast<int>(ppm.stat[COL][c]);
	}
	stream<< std::endl;
	for (size_t r = 0; r < ppm.m.R; ++r) {
		
		if (ppm.stat[ROW][r] == status::red)
			stream << RED << "R";
		else if (ppm.stat[ROW][r] == status::blue)
			stream << BLUE << "B";
		else if (ppm.stat[ROW][r] == status::cut)
			stream<< NONE << "C";
		else	stream << NONE << static_cast<int>(ppm.stat[ROW][r]);
		
		stream << NONE;
		for (size_t c = 0, j = 0; c < ppm.m.C; ++c) {
			if (j < ppm.m.rows[r].size() && ppm.m.rows[r][j]->c == c) {
				++j;
				if (ppm.stat[COL][c] == status::red ||
					ppm.stat[ROW][r] == status::red)
					stream << RED;
				else if (ppm.stat[COL][c] == status::blue ||
					ppm.stat[ROW][r] == status::blue)
					stream << BLUE;
				stream << "#" << NONE;
					
			} else stream << '.';
		}
		
		stream << std::endl;
	}
	stream << RED << ppm.partition_size[0] << NONE << '/' << BLUE << ppm.partition_size[1] << NONE << '/' << ppm.max_partition_size << std::endl;
	stream << ppm.cut << '/' << ppm.implicitely_cut << std::endl;
	
	return stream;
}

