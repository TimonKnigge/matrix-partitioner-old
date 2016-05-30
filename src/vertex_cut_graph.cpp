#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <iostream>

#include "resettable_array.cpp" // This .cpp include is necessary because of template problems (see `linking issue' in http://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp)
#include "vertex_cut_graph.h"


void vertex_cut_graph::add_edge(size_t u, size_t v, size_t cap) {
	E[outvertex(u)].push_back(vc_edge( invertex(v), E[ invertex(v)].size()    , cap));
	E[ invertex(v)].push_back(vc_edge(outvertex(u), E[outvertex(u)].size() - 1, 0  ));
}

size_t vertex_cut_graph::maximal_vertex_cut() {
	return flow;
}

void vertex_cut_graph::add_source(size_t u) {
	active[invertex(u)] = active[outvertex(u)] = true;
	E[invertex(u)][0].cap = 1;
	
	u = outvertex(u);
	
	std::set<std::pair<size_t, size_t>> _so;
	_so.insert({u, 0});
	
	size_t inc = 0;
	while (true)
		if (find_flow(_so, sinks) == 0)
			break;
		else	++inc;
	
	flow += inc;
	sources.insert({u, inc});
}

void vertex_cut_graph::remove_source(size_t u) {
	u = outvertex(u);
	
	auto it = sources.lower_bound({u, 0});
	std::set<std::pair<size_t, size_t>> _so;
	_so.insert(*it);
	size_t excess = it->second;
	flow -= excess;
	sources.erase(it);
	
	// There is <excess> outgoing flow at u, try to get it
	// from different sources
	while (excess > 0 && find_flow(sources, _so) > 0) --excess, ++flow;
	
	// If we couldn't rewire all flow, we'll have to move some back
	while (excess > 0) find_flow(sinks, _so, -1), --excess;
}

void vertex_cut_graph::add_sink(size_t u) {
	
	active[invertex(u)] = active[outvertex(u)] = true;
	E[invertex(u)][0].cap = 1;
	
	u = invertex(u);
	
	std::set<std::pair<size_t, size_t>> _si;
	_si.insert({u, 0});
	
	size_t inc = 0;
	while (true)
		if (find_flow(sources, _si) == 0)
			break;
		else	++inc;
	
	flow += inc;
	sinks.insert({u, inc});
}

void vertex_cut_graph::remove_sink(size_t u) {
	
	u = invertex(u);
	
	auto it = sinks.lower_bound({u, 0});
	std::set<std::pair<size_t, size_t>> _si;
	_si.insert(*it);
	size_t excess = it->second;
	flow -= excess;
	sinks.erase(it);
	
	// There is <excess> incoming flow at u, try to get it
	// to different sinks
	while (excess > 0 && find_flow(_si, sinks) > 0) --excess, ++flow;
	
	// If we couldn't rewire all flow, we'll have to move some back
	while (excess > 0) find_flow(_si, sources, -1), --excess;
	
}

bool vertex_cut_graph::is_active(size_t u) {
	return active[u];
}

void vertex_cut_graph::set_activity(size_t u, bool activity) {
	size_t inv = invertex(u), outv = outvertex(u);
	if (active[inv] == activity) return;
	
	if (active[inv]) {
		// u is set to inactive
		
		// Is there flow passing through u? The first outgoing
		// edge of inv goes to outv.
		if (E[inv][0].flow == 0) {
			active[inv] = active[outv] = false;
			return;
		}
		
		// Reset the flow and capacity in inv
		E[inv][0].flow = E[inv][0].cap = 0;
		E[outv][0].flow = E[outv][0].cap = 0;
		
		// Try to push flow from inv to outv over some other path
		std::set<std::pair<size_t, size_t>> _so, _si;
		_so.insert({inv, 1});
		_si.insert({outv, 1});
		if (find_flow(_so, _si) == 1) {
			active[inv] = active[outv] = false;
			return;
		}

		// We'll have to remove this path. Push back
		// from inv to some source and from some sink to outv
		find_flow(_so, sources, -1);
		find_flow(sinks, _si, -1);
		--flow;
		active[inv] = active[outv] = false;
	} else {
		// u is set to active
		// Fix capacities and status
		active[inv] = active[outv] = true;
		E[inv][0].cap = 1;
		
		flow += find_flow(sources, sinks);
	}
}

size_t vertex_cut_graph::find_flow(
	std::set<std::pair<size_t, size_t>> &_sources,
	std::set<std::pair<size_t, size_t>> &_sinks,
	int coeff) {
	
	parent.reset_all();
	parent_edge.reset_all();
	std::queue<size_t> q;
	for (std::pair<size_t, size_t> _source : _sources) {
		if (_source.second > 0 || coeff > 0) {
			q.push(_source.first);
			parent.set(_source.first, -2);
		}
		// We'll use -2 to denote 'no parent, this is a source'
		// (as opposed to -1 which denotes 'no parent yet')
	}
	
	int _sink = -1;
	while (!q.empty()) {
		size_t u = q.front();
		q.pop();
		
		auto it = _sinks.lower_bound({u, 0});
		if (it != _sinks.end() && it->first == u && (coeff > 0 || it->second > 0)) {
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

	int u = _sink, _source = _sink;
	while (u != -2) {
		int p = parent.get(u), pi = parent_edge.get(u);
		if (p == -2) {
			_source = u;
			break;
		}
		
		E[p][pi].flow++;
		E[u][E[p][pi].rev].flow--;
		
		u = p;
	}
	
	auto it = _sources.lower_bound({_source, 0});
	std::pair<size_t, size_t> new_source = {_source, it->second + coeff};
	_sources.erase(it);
	_sources.insert(new_source);
	it = _sinks.lower_bound({_sink, 0});
	std::pair<size_t, size_t> new_sink = {_sink, it->second + coeff};
	_sinks.erase(it);
	_sinks.insert(new_sink);
	
	return 1;
}
