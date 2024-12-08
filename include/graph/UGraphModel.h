/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
     //class UGraphAlgorithm;
     //friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (!fromNode || !toNode)
            throw VertexNotFoundException("Vertex not found");

        // Connect both directions for undirected graph
        fromNode->connect(toNode, weight);
        toNode->connect(fromNode, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (!fromNode || !toNode){
            throw VertexNotFoundException("Vertex not found");
        }
        // toNode->inDegree_--;
        // fromNode->outDegree_--;
        fromNode->removeTo(toNode);
        toNode->removeTo(fromNode);
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *targetNode = this->getVertexNode(vertex);
        if (!targetNode)
            throw VertexNotFoundException("Vertex not found");

        // Remove all edges connected to this vertex
        DLinkedList<T> edges = targetNode->getOutwardEdges();
        typename DLinkedList<T>::Iterator it = edges.begin();
        while (it != edges.end())
        {
            typename AbstractGraph<T>::VertexNode *neighbor = this->getVertexNode(*it);
            neighbor->removeTo(targetNode);
            ++it;
        }

        // Remove vertex
        this->nodeList.removeItem(targetNode);
        delete targetNode;
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

        // Add all vertices
        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }

        // Add all edges
        for (int i = 0; i < nedges; i++)
        {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
