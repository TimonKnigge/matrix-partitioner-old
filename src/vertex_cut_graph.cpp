#include <vector>
#include <set>
#include <algorithm>
#include <queue>

#include "resettable_array.cpp" // This .cpp include is necessary because of template problems (see `linking issue' in http://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp)
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

size_t vertex_cut_graph::find_flow(
	std::set<std::pair<size_t, size_t>> &_sources,
	std::set<std::pair<size_t, size_t>> &_sinks) {
	
	parent.reset_all();
	parent_edge.reset_all();
	std::queue<size_t> q;
	for (std::pair<size_t, size_t> _source : _sources) {
		q.push(_source.first);
		parent.set(_source.first, -2);
		// We'll use -2 to denote 'no parent, this is a source'
		// (as opposed to -1 which denotes 'no parent yet')
	}
	
	int _sink = -1;
	while (!q.empty()) {
		size_t u = q.front();
		q.pop();
		
		auto it = _sinks.lower_bound({u, 0});
		if (it != _sinks.end() && it->first == u) {
			_sink = int(u);
			break;
		}
		
		for (size_t i = 0; i < E[u].size(); ++i) {
			const vc_edge &edge = E[u][i];
			if (edge.flow == edge.cap) continue;
			if (!active[edge.v]) continue;
			if (parent.get(edge.v) != -1) continue;
			
			parent.set(edge.v, u);
			parent_edge.set(edge.v, i);
			q.push(edge.v);
		}
	}
	
	if (_sink == -1) return 0;
	
	while (_sink != -2) {
		int p = parent.get(_sink), pi = parent_edge.get(_sink);
		
		E[p][pi].flow++;
		E[_sink][E[p][pi].rev].flow--;
		
		_sink = p;
	}
	
	return 1;
}
