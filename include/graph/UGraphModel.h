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

        if (!fromNode ){
            throw VertexNotFoundException(this->vertex2str(from));
        }
        if ( !toNode){
            throw VertexNotFoundException(this->vertex2str(to));
        }
        // Connect both directions for undirected graph
        fromNode->connect(toNode, weight);
        toNode->connect(fromNode, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        if (!fromNode )
            throw VertexNotFoundException(this->vertex2str(from));
        if ( !toNode)
            throw VertexNotFoundException(this->vertex2str(to));
        if (!this->connected(from, to))
        throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");
    
        // toNode->inDegree_--;
        // fromNode->outDegree_--;
        fromNode->removeTo(toNode);
        toNode->removeTo(fromNode);
    }
    void remove(T vertex)
    {
        typename AbstractGraph<T>::VertexNode* vertexNode = this->getVertexNode(vertex);
    if(!vertexNode) 
        throw VertexNotFoundException(this->vertex2str(vertex));
    
    // First remove all edges connected to this vertex
    typename DLinkedList<typename AbstractGraph<T>::VertexNode*>::Iterator nodeIt = this->nodeList.begin();
    while(nodeIt != this->nodeList.end()) {
        typename AbstractGraph<T>::VertexNode* node = *nodeIt;
        if(node != vertexNode) {
            // Remove edges in both directions
            node->removeTo(vertexNode);
            vertexNode->removeTo(node);
        }
        nodeIt++;
    }
    
    // Handle self-loops
    if(this->connected(vertex, vertex)) {  // Fixed: added this->
        vertexNode->removeTo(vertexNode);
    }
    
    // Finally remove the vertex
    this->nodeList.removeItem(vertexNode);
    delete vertexNode;
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
