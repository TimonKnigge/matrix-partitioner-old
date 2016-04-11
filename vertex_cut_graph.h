/*
	vertex_cut_graph.h
	
	Representation of a vertex_cut_graph. This is a very specialised
	implementation specifically intended for finding maximal vertex
	cuts between sets of vertices, and dynamic updates.
*/

#pragma once

#include <vector>
#include <set>

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
	const size_t N, V, flow = 0;
	
	// The edgelist of the graph
	std::vector<std::vector<vc_edge>> E;
	
	// Denotes for each vertex whether it is active (i.e. should
	// be taken into account for finding the maximal vertex cut).
	std::vector<bool> active;
	
	// The current set of source/sink vertices
	std::set<size_t> source, sink;
	
	flow_graph(size_t N)
		: N(N), V(2 * N), E(2 * N, std::vector<vc_edge>()),
		  active(2 * N, true) {
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
	void update_flow(set<size_t> &sources, set<size_t> &sinks);
	
	static size_t  invertex(size_t u) { return 2 * u;     }
	static size_t outvertex(size_t u) { return 2 * u + 1; }
};

