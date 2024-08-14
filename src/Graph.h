#include <type_traits>
#include <unordered_map>
#include <set>
#include <limits>
#include <iostream>
#include <queue>

#pragma once

namespace graph
{
	template <typename W, typename V>
	struct Edge
	{
		Edge() {}
		Edge(W w_, V to_)
			: w(w_), to(to_) {}
		W w;
		V to;
	};


	template <typename W, typename V>
	class Graph;

	template <typename W, typename V>
	std::unordered_map<V, W> Dijkstra(V from, Graph<W, V>& g);

	template <typename W, typename V>
	std::unordered_map<V, W> Bellman_Ford(V from, Graph<W, V>& g);

	template <typename W, typename V>
	std::pair<std::vector<V>, W> findPath(V from, V to, Graph<W, V> graph);

	template <typename W = unsigned int, typename V = int>
	class Graph
	{
		friend std::unordered_map<V, W>		Bellman_Ford<W, V>(V from, Graph<W, V>& g);
		friend std::unordered_map<V, W>		Dijkstra<W, V>(V from, Graph<W, V>& g);
		friend std::pair<std::vector<V>, W> findPath<W, V>(V from, V to, Graph<W, V> graph);

	public:
		void addEdge(V n1, V n2, W weight)
		{
			nodes.insert(n1);
			nodes.insert(n2);

			edges_to.insert(std::make_pair(n1, Edge<W, V>(weight, n2)));
			edges_from.insert(std::make_pair(n2, Edge<W, V>(weight, n1)));
		}

		size_t getNodesCount() { return nodes.size(); }

	private:
		std::unordered_multimap<V, Edge<W, V>> edges_to;
		std::unordered_multimap<V, Edge<W, V>> edges_from;
		std::set<V>							   nodes;
	};

	template <typename W = unsigned int, typename V = int>
	std::pair<std::vector<V>, W> findPath(V from, V to, Graph<W, V> graph)
	{
		std::unordered_map<V, W> res;

		if constexpr (std::is_unsigned_v<W>)
		{
			res = Dijkstra<W, V>(from, graph);
		}
		else
		{
			res = Bellman_Ford<W, V>(from, graph);
		}

		std::pair<std::vector<V>, W> ret;
		ret.second = res[to];

		std::unordered_multimap<V, Edge<W, V>>& edges_from = graph.edges_from;

		V current = to;
		while (current != from)
		{
			auto range = edges_from.equal_range(current);
			for (auto it = range.first; it != range.second; ++it)
			{
				if (res[current] == it->second.w + res[it->second.to])
				{
					if (from != it->second.to)
					{
						ret.first.push_back(it->second.to);
					}
					current = it->second.to;
					break;
				}
			}
		}
		if (ret.first.size() > 1)
		{
			std::reverse(ret.first.begin(), ret.first.end());
		}

		return ret;
	}

	template <typename W, typename V>
	std::unordered_map<V, W> Dijkstra(V from, Graph<W, V>& g)
	{
		std::unordered_map<V, W>																		  costs;
		std::priority_queue<std::pair<W, V>, std::vector<std::pair<W, V>>, std::greater<std::pair<W, V>>> heap;
		std::set<V>																						  nodes_remain = g.nodes;
		for (auto i : g.nodes)
		{
			costs[i] = std::numeric_limits<W>::max();
		}
		costs[from] = 0;

		heap.push(std::make_pair(0, from));

		while (!heap.empty())
		{
			auto top = heap.top();
			heap.pop();
			nodes_remain.erase(top.second);
			if (costs[top.second] > top.first)
			{
				costs[top.second] = top.first;
			}
			auto range = g.edges_to.equal_range(top.second);
			for (auto it = range.first; it != range.second; ++it)
			{
				if (nodes_remain.count(it->second.to) == 1)
				{
					heap.push(std::make_pair(it->second.w + costs[top.second], it->second.to));
				}
			}
		}
		return costs;
	}

	template <typename W, typename V>
	std::unordered_map<V, W> Bellman_Ford(V from, Graph<W, V>& g)
	{
		std::unordered_map<V, W> res;
		for (auto i : g.nodes)
		{
			res[i] = std::numeric_limits<W>::max();
		}
		res[from] = 0;

		for (size_t i = 0; i < g.getNodesCount() - 1; ++i)
		{
			for (auto it = g.edges_to.begin(); it != g.edges_to.end(); ++it)
			{
				auto& edge = *it;
				if (res[edge.first] == std::numeric_limits<W>::max())
				{
					continue;
				}

				W new_weight = res[edge.first] + edge.second.w;
				if (res[edge.second.to] > new_weight)
				{
					res[edge.second.to] = new_weight;
				}
			}
		}

		for (auto it = g.edges_to.begin(); it != g.edges_to.end(); ++it)
		{
			auto& edge = *it;
			if (res[edge.first] == std::numeric_limits<W>::max() || res[edge.second.to] > (res[edge.first] + edge.second.w))
			{
				throw std::exception("Graph contains negative weight cycle");
			}
		}

		return res;
	}

} // namespace graph