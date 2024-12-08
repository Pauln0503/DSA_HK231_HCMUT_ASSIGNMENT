/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
 #include "stacknqueue/Queue.h"
 #include "stacknqueue/Stack.h"
// #include "hash/XHashMap.h"
// #include "stacknqueue/PriorityQueue.h"
 #include "sorting/DLinkedListSE.h"
 #include "stacknqueue/IDeck.h"
#include "hash/xMap.h"
#include "sorting/ISort.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
public:
    DGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0) override
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        if (!fromNode)
        {
            throw VertexNotFoundException("Vertex " + this->vertex2str(from) + " not found.");
        }

        // Lấy VertexNode của đỉnh to
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);
        if (!toNode)
        {
            throw VertexNotFoundException("Vertex " + this->vertex2str(to) + " not found.");
        }

        // Kết nối từ from đến to
        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to) override
    {
        // Lấy VertexNode của đỉnh from
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        if (!fromNode)
        {
            throw VertexNotFoundException("Vertex " + this->vertex2str(from) + " not found.");
        }
        // Lấy VertexNode của đỉnh to
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);
        if (!toNode)
        {
            throw VertexNotFoundException("Vertex " + this->vertex2str(to) + " not found.");
        }
        // Xóa cạnh từ from đến to
        typename AbstractGraph<T>::Edge *edge = fromNode->getEdge(toNode);
        if (!edge)
        {
            throw EdgeNotFoundException("Edge from " + this->vertex2str(from) + " to " + this->vertex2str(to) + " not found.");
        }
        fromNode->removeTo(toNode);
    }
    void remove(T vertex) override
    {
        typename AbstractGraph<T>::VertexNode *targetNode = this->getVertexNode(vertex);
        if (!targetNode)
        {
            throw VertexNotFoundException("Vertex " + this->vertex2str(vertex) + " not found.");
        }

        // Remove incoming edges
        typename DLinkedList<typename AbstractGraph<T>::VertexNode *>::Iterator it = this->nodeList.begin();
        while (it != this->nodeList.end())
        {
            typename AbstractGraph<T>::VertexNode *node = *it;
            if (node != targetNode)
            {
                node->removeTo(targetNode);
            }
            ++it;
        }

        // Remove outgoing edges
        DLinkedList<T> outEdges = targetNode->getOutwardEdges();
        typename DLinkedList<T>::Iterator outIt = outEdges.begin();
        while (outIt != outEdges.end())
        {
            T toVertex = *outIt;
            targetNode->removeTo(this->getVertexNode(toVertex));
            ++outIt;
        }

        // Remove the vertex
        this->nodeList.removeItem(targetNode);
        delete targetNode;
    }

    static DGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        DGraphModel<T> *graph = new DGraphModel<T>(vertexEQ, vertex2str);

        // Thêm tất cả các đỉnh vào đồ thị
        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }

        // Thêm tất cả các cạnh vào đồ thị
        for (int i = 0; i < nedges; i++)
        {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
