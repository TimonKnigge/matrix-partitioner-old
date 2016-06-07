/*
	vertex_cut_graph.h
	
	Representation of a vertex_cut_graph. This is a very specialised
	implementation specifically intended for finding maximal vertex
	cuts between sets of vertices, and dynamic updates.
*/

#pragma once

#include <vector>
#include <set>

#include "resettable_array.h"

// A directed edge in the flow graph.
struct vc_edge{
	const size_t v;		// Index of the endpoint of this edge.
	const size_t rev;	// Index of the reverse (residual) edge.
	int flow = 0, cap;	// Flow and capacity of this edge.
	vc_edge(size_t v, size_t rev, int cap) :
		v(v), rev(rev), cap(cap) { }
};

class vertex_cut_graph {
	
public:
	
	// The number of vertices in the flow graph and the current
	// maximal flow
	const size_t N, V;
	int flow = 0;
	
	// The edgelist of the graph
	std::vector<std::vector<vc_edge>> E;
	
	// Denotes for each vertex whether it is active (i.e. should
	// be taken into account for finding the maximal vertex cut).
	std::vector<bool> active;
	
	// The current set of source/sink vertices (a tuple (vertex id, flow))
	std::set<std::pair<size_t, size_t>> sources, sinks;
	
	vertex_cut_graph(size_t N)
		: N(N), V(2 * N), E(2 * N, std::vector<vc_edge>()),
		  active(2 * N, true),
		  parent(2 * N, -1), parent_edge(2 * N, 0) {
		for (size_t i = 0; i < N; ++i) {
			E[ invertex(i)].push_back(vc_edge(outvertex(i), 0, 1));
			E[outvertex(i)].push_back(vc_edge( invertex(i), 0, 0));
		}
	}
	
	// Add an edge to the flow graph
	void add_edge(size_t u, size_t v, size_t cap);
	
	// Returns the maximal vertex cut.
	size_t maximal_vertex_cut();
	
	// Getting and setting whether a vertex is active
	bool is_active(size_t u);
	void set_activity(size_t u, bool activity);
	
	// Add/remove source/sink vertices
	void add_source(size_t u);
	void remove_source(size_t u);
	void add_sink(size_t u);
	void remove_sink(size_t u);

private:
	resettable_array<int> parent, parent_edge;
	
	size_t find_flow(std::set<std::pair<size_t, size_t>> &_sources, std::set<std::pair<size_t, size_t>> &_sinks, int coeff = 1);
	size_t find_reverse_flow(std::set<std::pair<size_t, size_t>> &_sources, std::set<std::pair<size_t, size_t>> &_sinks, int coeff = 1);
	
	static size_t  invertex(size_t u) { return 2 * u;     }
	static size_t outvertex(size_t u) { return 2 * u + 1; }
};

