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
    DLinkedList<T> result;
    DLinkedListSE<T> allVertices = graph->vertices();

    // Sort vertices in descending order if required
    if (sorted)
    {
        // Sorting in descending order by reversing the order after sorting in ascending
        //allVertices.sort();
        allVertices.reverseSort();  // Reverse the list after sorting in ascending order
    }
    //cout << allVertices.toString() << endl;
    // Default hash code if not provided
    if (!hash_code)
    {
        hash_code = [](T &item, int size) -> int
        {
            return item % size;
        };
    }

    // Initialize visited map with int (0 = not visited, 1 = visited)
    xMap<T, int> visited(hash_code);

    // Initialize all vertices as not visited (0 means not visited)
    typename DLinkedList<T>::Iterator initIt = allVertices.begin();
    while (initIt != allVertices.end())
    {
        visited.put(*initIt, 0);  // 0 means not visited
        initIt++;
    }

    // DFS function
    // Duyệt các đỉnh con theo thứ tự giảm dần
std::function<void(T)> dfs = [&](T vertex) {
    if (visited.get(vertex) == 1) // Đã duyệt
        return;

    visited.put(vertex, 1);  // Đánh dấu đã duyệt

    // Lấy danh sách các đỉnh kề
    DLinkedListSE<T> adjacents = graph->getOutwardEdges(vertex);
    
    // Sắp xếp giảm dần
    adjacents.reverseSort();

    // Đệ quy DFS cho các đỉnh kề
    typename DLinkedList<T>::Iterator adjIt = adjacents.begin();
    while (adjIt != adjacents.end()) {
        dfs(*adjIt);
        adjIt++;
    }

    // Thêm đỉnh vào kết quả sau khi duyệt (Post-order)
    result.add(0, vertex);  // Thêm vào đầu danh sách kết quả
};


    // // Use listOfZeroInDegrees to start traversal
     DLinkedListSE<T> zeroInDegrees = listOfZeroInDegrees();

    // Reverse the zeroInDegrees list manually
    DLinkedListSE<T> reversedZeroInDegrees;
    typename DLinkedListSE<T>::Iterator zeroIt = zeroInDegrees.begin();
    while (zeroIt != zeroInDegrees.end())
    {
        reversedZeroInDegrees.add(0, *zeroIt); // Add to the front to reverse
        zeroIt++;
    }

    zeroInDegrees = reversedZeroInDegrees; // Assign back the reversed list

    // Traverse vertices starting from zero in-degree vertices
    typename DLinkedListSE<T>::Iterator vertexIt = zeroInDegrees.begin();
    while (vertexIt != zeroInDegrees.end())
    {
        dfs(*vertexIt);
        vertexIt++;
    }

    // Ensure all vertices are processed (handle disconnected components)
    typename DLinkedList<T>::Iterator allVertexIt = allVertices.begin();
    while (allVertexIt != allVertices.end())
    {
        if (visited.get(*allVertexIt) == 0)  // 0 means not visited
        {
            dfs(*allVertexIt);
        }
        allVertexIt++;
    }

    return result;
}




// }
// DLinkedList<T> dfsSort(bool sorted = true)
// {
//     DLinkedList<T> result;
//     DLinkedListSE<T> allVertices = graph->vertices();

//     if (!hash_code)
//     {
//         hash_code = [](T &item, int size) -> int
//         {
//             return item % size;
//         };
//     }

//     // Initialize visited map
//     xMap<T, bool> visited(hash_code);

//     // Initialize all vertices as not visited
//     typename DLinkedList<T>::Iterator initIt = allVertices.begin();
//     while (initIt != allVertices.end())
//     {
//         visited.put(*initIt, false);
//         initIt++;
//     }

//     // DFS function
//     std::function<void(T)> dfs = [&](T vertex)
//     {
//         // Mark as visited
//         visited.put(vertex, true);

//         // Get outward edges
//         DLinkedList<T> adjacents = graph->getOutwardEdges(vertex);
//         typename DLinkedList<T>::Iterator it = adjacents.begin();

//         while (it != adjacents.end())
//         {
//             T adjacent = *it;
//             // Check if not visited before recursing
//             if (!visited.get(adjacent))
//             {
//                 dfs(adjacent);
//             }
//             it++;
//         }

//         // Add to the front of the list for correct topological order
//         result.add(0, vertex);
//     };

//     // Traverse all vertices
//     typename DLinkedList<T>::Iterator vertexIt = allVertices.begin();
//     while (vertexIt != allVertices.end())
//     {
//         T vertex = *vertexIt;
//         if (!visited.get(vertex))
//         {
//             dfs(vertex);
//         }
//         vertexIt++;
//     }

//     // Create a list of vertices with in-degree of 0
//     DLinkedList<T> zeroInDegreeVertices;
//     typename DLinkedList<T>::Iterator it = allVertices.begin();
//     while (it != allVertices.end())
//     {
//         if (graph->inDegree(*it) == 0)
//         {
//             zeroInDegreeVertices.add(*it);
//         }
//         it++;
//     }

//     // Reverse the order of the vertices with in-degree of 0 in the result
//     typename DLinkedList<T>::Iterator reverseIt = zeroInDegreeVertices.begin();
//     while (reverseIt != zeroInDegreeVertices.end())
//     {
//         T vertex = *reverseIt;
//         // Remove the vertex from the result and add it back to the front
//         result.removeAt(vertex);
//         result.add(0, vertex);
//         reverseIt++;
//     }

//     return result;
// }




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
        //cout << "Zero In-Degrees List: " << zeroInDegrees.toString() << endl;
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
