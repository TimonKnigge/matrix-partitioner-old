#include <vector>
#include <set>
#include <algorithm>

#include "vertex_cut_graph.h"


void vertex_cut_graph::add_edge(size_t u, size_t v, size_t cap) {
	E[u].push_back(v, E[v].size()    , cap);
	E[v].push_back(u, E[u].size() - 1, 0  );
}

size_t vertex_cut_graph::maximal_vertex_cut() {
	return flow;
}

bool is_active(size_t u) {
	return active[u];
}
