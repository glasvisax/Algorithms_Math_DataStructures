#include <type_traits>
#include <unordered_map>
#include <set>
#include <limits>
#include <iostream>
#include <queue>
#include <stack>
#include <deque>

#pragma once

namespace graph
{
	template <typename W = unsigned int, typename V = int>
	struct WGraph
	{
		using Edge = std::pair<W, V>;
		using EdgesArray = std::unordered_multimap<V, Edge>;
		using NodesArray = std::set<V>;

		void addEdge(V n1, V n2, W weight)
		{
			nodes.insert(n1);
			nodes.insert(n2);

			edges_to.insert(std::make_pair(n1, Edge(weight, n2)));
			edges_from.insert(std::make_pair(n2, Edge(weight, n1)));
		}

		static V getEdgeDirection(Edge e)
		{
			return e.second;
		}

		static W getEdgeWeight(Edge e)
		{
			return e.first;
		}

		size_t getNodesCount() const { return nodes.size(); }

		EdgesArray edges_to;
		EdgesArray edges_from;
		NodesArray nodes;
	};

	template <typename V = int>
	struct UGraph
	{
		using Edge = V;
		using EdgesArray = std::unordered_multimap<V, Edge>;
		using NodesArray = std::set<V>;

		void addEdge(V n1, V n2)
		{
			nodes.insert(n1);
			nodes.insert(n2);

			edges_to.insert(std::make_pair(n1, Edge(n2)));
			edges_from.insert(std::make_pair(n2, Edge(n1)));
		}

		size_t getNodesCount() const { return nodes.size(); }

		static V getEdgeDirection(Edge e)
		{
			return e;
		}

		EdgesArray edges_to;
		EdgesArray edges_from;
		NodesArray nodes;
	};

	template <typename W = unsigned int, typename V = int>
	std::pair<std::vector<V>, W> findPath(V from, V to, const WGraph<W, V>& graph)
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
		ret.first.push_back(to);

		const typename WGraph<W, V>::EdgesArray& edges_from = graph.edges_from;

		V current = to;
		while (current != from)
		{
			auto range = edges_from.equal_range(current);
			for (auto it = range.first; it != range.second; ++it)
			{
				if (res[current] == it->second.first + res[it->second.second])
				{
					if (from != it->second.second)
					{
						ret.first.push_back(it->second.second);
					}
					current = it->second.second;
					break;
				}
			}
		}
		ret.first.push_back(from);
		if (ret.first.size() > 1)
		{
			std::reverse(ret.first.begin(), ret.first.end());
		}

		return ret;
	}

	template <typename V = unsigned int>
	std::pair<std::vector<V>, size_t> findPath(V from, V to, const UGraph<V>& graph, bool bfs = true)
	{
		if (bfs)
		{
			auto vec = BFS<V, UGraph<V>>(from, graph, to);
			size_t size = vec.size();
			auto   res = std::make_pair(std::move(vec), size);
			return res;
		}
		else
		{
			auto   vec = DFS<V, UGraph<V>>(from, graph, to);
			size_t size = vec.size();
			auto   res = std::make_pair(std::move(vec), size);
			return res;
		}
	}

	template <typename V = int, typename Graph>
	std::vector<std::vector<V>> BFS(V from, const Graph& graph)
	{
		std::deque<std::vector<V>>	fifo;
		std::vector<std::vector<V>> res;
		typename Graph::NodesArray	nodes_unvisited = graph.nodes;
		fifo.push_front(std::vector<V>(1, from));
		while (!fifo.empty())
		{
			auto& vec = fifo.front();
			V	  node = vec.back();
			auto  range = graph.edges_to.equal_range(node);
			auto  it = range.first;

			if (it == range.second)
			{
				goto continue_flow;
			}
			V next = Graph::getEdgeDirection(it->second);
			while (nodes_unvisited.find(next) == nodes_unvisited.end())
			{
				++it;
				if (it == range.second)
				{
					goto continue_flow;
				}
				next = Graph::getEdgeDirection(it->second);
			}
			vec.push_back(next);
			it++;
			for (; it != range.second; ++it)
			{
				auto next = Graph::getEdgeDirection(it->second);
				if (nodes_unvisited.find(next) != nodes_unvisited.end())
				{
					fifo.push_back(std::vector<V>(fifo[0]));
					fifo[0][fifo[0].size() - 1] = next;
				}
			}
			nodes_unvisited.erase(node);
			continue;

		continue_flow:
			res.push_back(std::move(vec));
			fifo.pop_front();
			continue;
		}
		return res;
	}

	template <typename V = int, typename Graph>
	std::vector<V> BFS(V from, const Graph& graph, V to)
	{
		std::deque<std::vector<V>> fifo;
		typename Graph::NodesArray nodes_unvisited = graph.nodes;
		fifo.push_front(std::vector<V>(1, from));
		while (!fifo.empty())
		{
			auto& vec = fifo.front();
			V	  node = vec.back();
			auto  range = graph.edges_to.equal_range(node);
			auto  it = range.first;

			if (it == range.second)
			{
				goto continue_flow;
			}
			V next = Graph::getEdgeDirection(it->second);
			while (nodes_unvisited.find(next) == nodes_unvisited.end())
			{
				++it;
				if (it == range.second)
				{
					goto continue_flow;
				}
				next = Graph::getEdgeDirection(it->second);
			}
			vec.push_back(next);
			if (next == to)
			{
				return vec;
			}
			it++;
			for (; it != range.second; ++it)
			{
				auto next = Graph::getEdgeDirection(it->second);
				if (nodes_unvisited.find(next) != nodes_unvisited.end())
				{
					fifo.push_back(std::vector<V>(fifo[0]));
					fifo[0][fifo[0].size() - 1] = next;
					if (next == to)
					{
						return fifo[0];
					}
				}
			}
			nodes_unvisited.erase(node);
			continue;

		continue_flow:
			fifo.pop_front();
			continue;
		}
		return std::vector<V>();
	}

	template <typename V = int, typename Graph>
	std::vector<V> DFS(V from, const Graph& graph, V to)
	{
		std::stack<std::vector<V>> lifo;
		typename Graph::NodesArray nodes_unvisited = graph.nodes;
		lifo.push(std::vector<V>(1, from));
		while (!lifo.empty())
		{
			auto& vec = lifo.top();
			V	  node = vec.back();
			auto  range = graph.edges_to.equal_range(node);
			auto  it = range.first;

			if (it == range.second)
			{
				goto continue_flow;
			}
			V next = Graph::getEdgeDirection(it->second);
			while (nodes_unvisited.find(next) == nodes_unvisited.end())
			{
				++it;
				if (it == range.second)
				{
					goto continue_flow;
				}
				next = Graph::getEdgeDirection(it->second);
			}
			vec.push_back(next);
			
			if (next == to)
			{
				return vec;
			}

			it++;
			for (; it != range.second; ++it)
			{
				auto next = Graph::getEdgeDirection(it->second);
				if (nodes_unvisited.find(next) != nodes_unvisited.end())
				{
					lifo.push(std::vector<V>(lifo.top()));
					lifo.top()[lifo.top().size() - 1] = next;
					
					if (next == to)
					{
						return lifo.top();
					}
					
				}
			}
			nodes_unvisited.erase(node);
			continue;

		continue_flow:
			lifo.pop();
			continue;
		}
		return std::vector<V>();
	}
	template <typename V = int, typename Graph>
	std::vector<std::vector<V>> DFS(V from, const Graph& graph)
	{
		std::stack<std::vector<V>>	lifo;
		std::vector<std::vector<V>> res;
		typename Graph::NodesArray	nodes_unvisited = graph.nodes;
		lifo.push(std::vector<V>(1, from));
		while (!lifo.empty())
		{
			auto& vec = lifo.top();
			V	  node = vec.back();
			auto  range = graph.edges_to.equal_range(node);
			auto  it = range.first;

			if (it == range.second)
			{
				goto continue_flow;
			}
			V next = Graph::getEdgeDirection(it->second);
			while (nodes_unvisited.find(next) == nodes_unvisited.end())
			{
				++it;
				if (it == range.second)
				{
					goto continue_flow;
				}
				next = Graph::getEdgeDirection(it->second);
			}
			vec.push_back(next);
			it++;
			for (; it != range.second; ++it)
			{
				auto next = Graph::getEdgeDirection(it->second);
				if (nodes_unvisited.find(next) != nodes_unvisited.end())
				{
					lifo.push(std::vector<V>(lifo.top()));
					lifo.top()[lifo.top().size() - 1] = next;
				}
			}
			nodes_unvisited.erase(node);
			continue;

		continue_flow:
			res.push_back(lifo.top());
			lifo.pop();
			continue;
		}
		return res;
	}

	template <typename W, typename V>
	std::unordered_map<V, W> Dijkstra(V from, const WGraph<W, V>& g)
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
				if (nodes_remain.count(it->second.second) == 1)
				{
					heap.push(std::make_pair(it->second.first + costs[top.second], it->second.second));
				}
			}
		}
		return costs;
	}

	template <typename W, typename V>
	std::unordered_map<V, W> Bellman_Ford(V from, const WGraph<W, V>& g)
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

				W new_weight = res[edge.first] + edge.second.first;
				if (res[edge.second.second] > new_weight)
				{
					res[edge.second.second] = new_weight;
				}
			}
		}

		for (auto it = g.edges_to.begin(); it != g.edges_to.end(); ++it)
		{
			auto& edge = *it;
			if (res[edge.first] == std::numeric_limits<W>::max() || res[edge.second.second] > (res[edge.first] + edge.second.first))
			{
				throw std::exception("Graph contains negative weight cycle");
			}
		}

		return res;
	}

} // namespace graph