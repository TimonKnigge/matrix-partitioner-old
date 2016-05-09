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

void vertex_cut_graph::update_flow(set<size_t> &sources, set<size_t> &sinks) {
	
}

void vertex_cut_graph::   add_source(size_t u){ source.insert(u);             }
void vertex_cut_graph::remove_source(size_t u){ source.erase(source.find(u)); }
void vertex_cut_graph::   add_sink  (size_t u){ sink  .insert(u);             }
void vertex_cut_graph::remove_sink  (size_t u){ sink  .erase(sink  .find(u)); }
