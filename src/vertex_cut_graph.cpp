#include <vector>
#include <set>
#include <algorithm>

#include "vertex_cut_graph.h"


void vertex_cut_graph::add_edge(size_t u, size_t v, size_t cap) {
	E[outvertex(u)].push_back(vc_edge( invertex(v), E[ invertex(v)].size()    , cap));
	E[ invertex(v)].push_back(vc_edge(outvertex(u), E[outvertex(u)].size() - 1, 0  ));
}

size_t vertex_cut_graph::maximal_vertex_cut() {
	return flow;
}

bool vertex_cut_graph::is_active(size_t u) {
	return active[u];
}

void vertex_cut_graph::set_activity(size_t u, bool activity) {
	if (active[u] == activity) return;
	if (active[u]) {
		// u is set to inactive
	}
}
