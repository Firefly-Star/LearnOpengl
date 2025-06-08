#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 3, 1)

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <crtdbg.h>
#include <0.0_Extentions/DataStructures/Allocator/PoolAllocator.h>

using namespace Firefly;

struct Node
{
	std::string name;
	std::string description;
	std::unordered_map<std::string, int> distance;
};

struct Edge
{
	std::string from;
	std::string to;
};

class Graph
{
public:
	using road_type = std::vector<Edge>;
	using map_type = std::unordered_map<std::string, std::pair<int, road_type>>;
	using record_type = std::unordered_map<std::string, PoolAllocatorHandle<Node>>;
	using pointer_type = PoolAllocatorHandle<Node>;

	std::pair<int, road_type> MinDistance(record_type const& nodes, std::string const& src, std::string const& dst);
	
	record_type Cin(PoolAllocator<Node>& allocator);

	void WriteFile(record_type const& nodes, std::string const& filename);
	
	record_type ReadFile(std::string const& filename, PoolAllocator<Node>& allocator);
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	PoolAllocator<Node> allocator;
	Graph g;

	 auto map = g.Cin(allocator);
	 g.WriteFile(map, "C:/Users/Summer/Desktop/OpenGL/c.txt");

	//auto map = g.ReadFile("C:/Users/Summer/Desktop/OpenGL/b.txt", allocator);

	for (auto const& [name, node] : map)
	{
		std::cout << node->name << ", " << node->description << "\n";
	}
	while (true)
	{
		std::cout << "是否继续查询(Y/N): ";
		char c;
		std::cin >> c;
		if (c == 'Y' || c == 'y')
		{
			std::cout << "输入源目名称: ";
			std::string src, dst;
			std::cin >> src >> dst;
			auto const& [dist, road] = g.MinDistance(map, src, dst);
			std::cout << "最短距离为: " << dist << "\n";
			std::cout << "最短路径为: \n";
			for (auto const& edge : road)
			{
				std::cout << edge.from << " -> " << edge.to << "\n";
			}
		}
		else
		{
			break;
		}
	}
}

Graph::record_type Graph::Cin(PoolAllocator<Node>& allocator)
{
	std::cout << "输入节点个数: ";
	int number;
	std::cin >> number;

	record_type record;

	while (number > 0)
	{
		std::cout << "输入节点(名称 描述): ";
		std::string name, description;
		std::cin >> name >> description;
		auto x = allocator.Allocate();
		x->name = name;
		x->description = description;
		record[name] = x;
		--number;
	}
	
	std::cout << "输入边的个数: ";
	std::cin >> number;
	while (number > 0)
	{
		std::cout << "输入边(源 目 距离): ";
		std::string src, dst;
		int distance;
		std::cin >> src >> dst >> distance;
		if (record.find(src) == record.end() || record.find(dst) == record.end())
		{
			std::cout << "无效的源/目\n";
		}
		else
		{
			record[src]->distance[dst] = distance;
			record[dst]->distance[src] = distance;
		}
		--number;
	}
	return record;
}

std::pair<int, Graph::road_type> Graph::MinDistance(record_type const& nodes, std::string const& src, std::string const& dst)
{
	if (nodes.find(src) == nodes.end() || nodes.find(dst) == nodes.end())
	{
		return { -1, road_type()};
	}
	map_type calculated;
	map_type uncalculated;

	pointer_type srcNode = nodes.at(src);
	pointer_type dstNode = nodes.at(dst);

	for (auto const& [name, node] : nodes)
	{
		if (node == srcNode)
		{
			uncalculated[node->name] = { 0, road_type() };
		}
		else
		{
			if (srcNode->distance.find(node->name) != srcNode->distance.end())
			{
				uncalculated[node->name] = { srcNode->distance[node->name], {{srcNode->name, node->name}} };
			}
			else
			{
				uncalculated[node->name] = { INT32_MAX, road_type() };
			}
		}
	}

	while (calculated.find(dstNode->name) == calculated.end())
	{
		pointer_type ptr;
		int temp = INT32_MAX;
		for (auto const& [node, distInfo] : uncalculated)
		{
			auto dist = distInfo.first;
			if (dist < temp)
			{
				ptr = nodes.at(node);
				temp = dist;
			}
		}
		calculated[ptr->name] = uncalculated[ptr->name];
		uncalculated.erase(ptr->name);
		for (auto const& [node, distInfo] : uncalculated)
		{
			auto dist = distInfo.first;
			if (ptr->distance.find(node) != ptr->distance.end())
			{
				if (dist > temp + ptr->distance[node])
				{
					road_type nr = calculated[ptr->name].second;
					nr.push_back({ ptr->name, node });
					uncalculated[node] = { temp + ptr->distance[node], nr };
				}
			}
		}
	}

	return calculated[dstNode->name];
}

void Graph::WriteFile(record_type const& nodes, std::string const& filename)
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file for writing");
	}

	file << "digraph G {\n";

	for (const auto& [name, node] : nodes)
	{
		file << "    \"" << node->name << "\" [label=\"" << node->name
			<< "\\n" << node->description << "\"];\n";
	}

	for (const auto& [name, node] : nodes)
	{
		for (const auto& [neighbor, dist] : node->distance)
		{
			file << "    \"" << node->name << "\" -> \"" << neighbor
				<< "\" [label=\"" << dist << "\"];\n";
		}
	}

	file << "}\n";
	file.close();
}

Graph::record_type Graph::ReadFile(std::string const& filename, PoolAllocator<Node>& allocator)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file for reading");
	}

	record_type node_map;

	std::string line;
	while (std::getline(file, line))
	{
		if (line != "digraph G {" && line != "}")
		{
			bool readingEdge = line.find("->") != std::string::npos;
			if (readingEdge)
			{
				std::string srcName, dstName;
				int distance = 0;
				{
					size_t srcNameStart = line.find("\"") + 1;
					size_t srcNameEnd = line.find("\"", srcNameStart);
					srcName = line.substr(srcNameStart, srcNameEnd - srcNameStart);

					size_t dstNameStart = line.find("\"", srcNameEnd + 1) + 1;
					size_t dstNameEnd = line.find("\"", dstNameStart);
					dstName = line.substr(dstNameStart, dstNameEnd - dstNameStart);

					size_t distStart = line.find("[label=\"", dstNameEnd + 1) + 8;
					size_t distEnd = line.find("\"", distStart);
					distance = std::stoi(line.substr(distStart, distEnd - distStart));
				}
				node_map[srcName]->distance[dstName] = distance;
			}
			else
			{
				std::string name, description;
				size_t nameStart = line.find("\"");
				size_t nameEnd = line.find("\"", nameStart + 1);
				name = line.substr(nameStart + 1, nameEnd - nameStart - 1);

				size_t labelStart = line.find("[label=\"") + 8;
				size_t labelEnd = line.find("]", labelStart + 1) - 1;
				size_t descriptionStart = line.find("\\n", labelStart + 1);
				description = line.substr(descriptionStart + 2, labelEnd - descriptionStart - 2);

				auto x = allocator.Allocate();
				x->name = name;
				x->description = description;
				node_map[name] = x;
			}
		}
	}

	file.close();
	return node_map;
}

#endif

