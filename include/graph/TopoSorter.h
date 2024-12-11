/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"

template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }
    DLinkedList<T> sort(int mode = 0, bool sorted = true)
    {
        // TODO

        return (mode == BFS) ? bfsSort(sorted) : dfsSort(sorted);
    }
    DLinkedList<T> bfsSort(bool sorted = true)
    {
        // Get list of vertices and optionally sort them
        DLinkedList<T> result;
        DLinkedListSE<T> allVertices = graph->vertices(); // Using DLinkedListSE for sorting
        // cout << "Initial vertices in dfsSort: " << allVertices.toString() << endl;
        //  Sort vertices if required
        if (sorted)
        {
            allVertices.sort();
        }
        // cout << "Initial vertices in dfsSort: " << allVertices.toString() << endl;
        if (!hash_code)
        {
            hash_code = [](T &item, int size) -> int
            {
                return item % size;
            };
        }
        // Get in-degrees for all vertices
        xMap<T, int> inDegrees = vertex2inDegree(hash_code);

        // Initialize queue with vertices having zero in-degree
        Queue<T> queue;
        DLinkedList<T> zeroInDegrees = listOfZeroInDegrees();
        typename DLinkedList<T>::Iterator it = zeroInDegrees.begin();
        while (it != zeroInDegrees.end())
        {
            queue.push(*it);
            it++;
        }

        // Process vertices in BFS order
        while (!queue.empty())
        {
            T current = queue.pop();
            result.add(current);

            // Process all neighbors
            DLinkedList<T> adjacents = graph->getOutwardEdges(current);
            typename DLinkedList<T>::Iterator adjIt = adjacents.begin();
            while (adjIt != adjacents.end())
            {
                T adjacent = *adjIt;
                int newInDegree = inDegrees.get(adjacent) - 1;
                inDegrees.put(adjacent, newInDegree);

                if (newInDegree == 0)
                {
                    queue.push(adjacent);
                }
                adjIt++;
            }
        }

        return result;
    }

   DLinkedList<T> dfsSort(bool sorted = true)
{
    xMap<T, bool> visited(hash_code); // Đánh dấu đỉnh đã thăm
    DLinkedList<T> Topo_list;         // Danh sách kết quả

    // Hàm DFS đệ quy
    std::function<void(const T &)> dfsVisit = [&](const T &vertex) {
        visited.put(vertex, true); // Đánh dấu đỉnh đã được thăm

        // Duyệt qua các đỉnh kề
        DLinkedList<T> neighbors = graph->getOutwardEdges(vertex);
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            T &neighbor = *it;
            if (!visited.containsKey(neighbor))
            {
                dfsVisit(neighbor); // Đệ quy với đỉnh kề
            }
        }

        // Sau khi duyệt xong, thêm đỉnh vào danh sách kết quả
        Topo_list.add(0, vertex); // Thêm vào đầu danh sách
    };

    // Duyệt toàn bộ các đỉnh trong đồ thị
    DLinkedList<T> vertices = graph->vertices();
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        T vertex = *it;
        if (!visited.containsKey(vertex))
        {
            dfsVisit(vertex);
        }
    }

    return Topo_list;
}



protected:
    xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        xMap<T, int> inDegreeMap(hash);
        DLinkedListSE<T> vertices = graph->vertices(); // Store vertices once
        typename DLinkedList<T>::Iterator it = vertices.begin();

        while (it != vertices.end())
        {
            inDegreeMap.put(*it, graph->inDegree(*it));
            it++;
        }
        return inDegreeMap;
    }

    xMap<T, int> vertex2outDegree(int (*hash)(T &, int))
    {
        xMap<T, int> outDegreeMap(hash);
        DLinkedList<T> vertices = graph->vertices(); // Lấy danh sách đỉnh

        // Debug: In ra danh sách vertices để kiểm tra
        // cout << "Vertices in vertex2outDegree: " << vertices.toString() << endl;

        typename DLinkedList<T>::Iterator it = vertices.begin();
        while (it != vertices.end())
        {
            T vertex = *it;
            int degree = graph->outDegree(vertex);
            outDegreeMap.put(vertex, degree);
            // cout << "Adding vertex " << vertex << " with degree " << degree << endl;
            it++;
        }
        return outDegreeMap;
    }

    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> zeroInDegrees;
        DLinkedList<T> vertices = graph->vertices(); // Store vertices once
        typename DLinkedList<T>::Iterator it = vertices.begin();

        while (it != vertices.end())
        {
            if (graph->inDegree(*it) == 0)
            {
                zeroInDegrees.add(*it);
            }
            it++;
        }
        // cout << "Zero In-Degrees List: " << zeroInDegrees.toString() << endl;
        return zeroInDegrees;
    }
};

// TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
