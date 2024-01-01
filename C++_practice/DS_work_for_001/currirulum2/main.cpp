#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include <fstream>

using namespace std;

// �ڵ����ͣ�����վ�����վ���û�
enum NodeType
{
  GENERATOR,
  SUBSTATION,
  USER
};

// ͼ�Ľڵ�
struct Node
{
  int id;
  NodeType type;

  Node(int _id, NodeType _type) : id(_id), type(_type) {}
};

// ͼ�ı�
struct Edge
{
  int from;
  int to;
  double cost;

  Edge(int _from, int _to, double _cost) : from(_from), to(_to), cost(_cost) {}
};

// ͼ��
class Graph
{
public:
  vector<Node> nodes;
  vector<Edge> edges;
  unordered_map<int, vector<Edge>> adjacencyList;

  void addNode(int id, NodeType type)
  {
    nodes.emplace_back(id, type);
  }

  void addEdge(int from, int to, double cost)
  {
    edges.emplace_back(from, to, cost);
    adjacencyList[from].emplace_back(from, to, cost);
    adjacencyList[to].emplace_back(to, from, cost); // ����ͼ��Ҫ˫���
  }

  void printAdjacencyList()
  {
    cout << "Adjacency List:" << endl;
    for (const auto &entry : adjacencyList)
    {
      cout << entry.first << ": ";
      for (const Edge &edge : entry.second)
      {
        cout << "(" << edge.from << "->" << edge.to << ", Cost: " << edge.cost << ") ";
      }
      cout << endl;
    }
  }

  void DFS(int startNode)
  {
    vector<bool> visited(nodes.size(), false);
    cout << "DFS Order: ";
    DFSUtil(startNode, visited);
    cout << endl;
  }

  void DFSUtil(int node, vector<bool> &visited)
  {
    cout << node << " ";
    visited[node] = true;

    for (const Edge &edge : edges)
    {
      if (edge.from == node && !visited[edge.to])
      {
        DFSUtil(edge.to, visited);
      }
    }
  }

  void BFS(int startNode)
  {
    vector<bool> visited(nodes.size(), false);
    queue<int> bfsQueue;

    cout << "BFS Order: ";

    bfsQueue.push(startNode);
    visited[startNode] = true;

    while (!bfsQueue.empty())
    {
      int node = bfsQueue.front();
      bfsQueue.pop();

      cout << node << " ";

      for (const Edge &edge : edges)
      {
        if (edge.from == node && !visited[edge.to])
        {
          bfsQueue.push(edge.to);
          visited[edge.to] = true;
        }
      }
    }

    cout << endl;
  }

  void generateDOTFile(const string &filename)
  {
    ofstream dotFile(filename);

    dotFile << "graph PowerNetwork {" << endl;

    for (const Edge &edge : edges)
    {
      dotFile << edge.from << " -- " << edge.to << " [label=\"" << edge.cost << "\"];" << endl;
    }

    dotFile << "}" << endl;

    dotFile.close();
  }

  void generateGraphImage(const string &dotFilename, const string &outputFilename)
  {
    string command = "dot -Tpng " + dotFilename + " -o " + outputFilename;
    system(command.c_str());
  }

  double findMinimumSpanningTreeCost()
  {
    priority_queue<Edge, vector<Edge>, function<bool(Edge, Edge)>> minHeap(
        [](Edge a, Edge b)
        { return a.cost > b.cost; });

    vector<bool> visited(nodes.size(), false);
    visited[0] = true;

    for (const Edge &edge : edges)
    {
      if (edge.from == 0)
      {
        minHeap.push(edge);
      }
    }

    double totalCost = 0.0;
    while (!minHeap.empty())
    {
      Edge minEdge = minHeap.top();
      minHeap.pop();

      if (!visited[minEdge.to])
      {
        visited[minEdge.to] = true;
        totalCost += minEdge.cost;

        for (const Edge &edge : edges)
        {
          if (edge.from == minEdge.to && !visited[edge.to])
          {
            minHeap.push(edge);
          }
        }
      }
    }

    return totalCost;
  }
};

int main()
{
  Graph powerNetwork;

  // ��ӽڵ�
  powerNetwork.addNode(0, GENERATOR);  // ����վ A
  powerNetwork.addNode(1, GENERATOR);  // ����վ B
  powerNetwork.addNode(2, SUBSTATION); // ���վ C
  powerNetwork.addNode(3, SUBSTATION); // ���վ D
  powerNetwork.addNode(4, USER);       // �û� E
  powerNetwork.addNode(5, USER);       // �û� F
  powerNetwork.addNode(6, USER);       // �û� G

  // ��ӱ�
  powerNetwork.addEdge(0, 2, 5.0); // A -> C, �ɱ� 5.0
  powerNetwork.addEdge(0, 3, 8.0); // A -> D, �ɱ� 8.0
  powerNetwork.addEdge(1, 2, 6.0); // B -> C, �ɱ� 6.0
  powerNetwork.addEdge(1, 3, 7.0); // B -> D, �ɱ� 7.0
  powerNetwork.addEdge(2, 4, 4.0); // C -> E, �ɱ� 4.0
  powerNetwork.addEdge(2, 5, 2.0); // C -> F, �ɱ� 2.0
  powerNetwork.addEdge(3, 6, 3.0); // D -> G, �ɱ� 3.0

  // ����ڽ��б�
  powerNetwork.printAdjacencyList();

  // ���ͼ�����������������
  powerNetwork.DFS(0);

  // ���ͼ�Ĺ��������������
  powerNetwork.BFS(0);

  // �����С�������ĳɱ�
  double minimumSpanningTreeCost = powerNetwork.findMinimumSpanningTreeCost();
  cout << "Minimum Spanning Tree Cost: " << minimumSpanningTreeCost << endl;

  return 0;
}
