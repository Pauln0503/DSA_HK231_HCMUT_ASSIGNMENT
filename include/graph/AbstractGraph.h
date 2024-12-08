/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
using namespace std;

template <class T>
class AbstractGraph : public IGraph<T>
{
public:
    class Edge;       // forward declaration
    class VertexNode; // forward declaration
    class Iterator;   // forward declaration

private:
protected:
    // Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode *> nodeList;

    // Function pointers:
    bool (*vertexEQ)(T &, T &); // to compare two vertices
    string (*vertex2str)(T &);  // to obtain string representation of vertices

    VertexNode *getVertexNode(T &vertex)
    {
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while (it != nodeList.end())
        {
            VertexNode *node = *it;
            if (vertexEQ(node->vertex, vertex))
                return node;
            it++;
        }
        return 0;
    }
    string vertex2Str(VertexNode &node)
    {
        return vertex2str(node.vertex);
    }
    string edge2Str(Edge &edge)
    {
        stringstream os;
        os << "E("
           << vertex2str(edge.from->vertex)
           << ","
           << vertex2str(edge.to->vertex)
           << ")";
        return os.str();
    }

public:
    AbstractGraph(
        bool (*vertexEQ)(T &, T &) = 0,
        string (*vertex2str)(T &) = 0)
    {

        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph() {}

    typedef bool (*vertexEQFunc)(T &, T &);
    typedef string (*vertex2strFunc)(T &);
    vertexEQFunc getVertexEQ()
    {
        return this->vertexEQ;
    }
    vertex2strFunc getVertex2Str()
    {
        return this->vertex2str;
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////// IMPLEMENTATION of IGraph API ////////////////////
    //////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight = 0) override = 0;
    virtual void disconnect(T from, T to) override = 0;
    virtual void remove(T vertex) override = 0;

    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex) override
    {
        // TODO
        if (contains(vertex))
        {
            return;
        }
        VertexNode *new_Node = new VertexNode(vertex, this->vertexEQ, this->vertex2str);
        nodeList.add(new_Node);
    }

    virtual bool contains(T vertex) override
    {
        // TODO
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while (it != nodeList.end())
        {
            VertexNode *node = *it;
            if (vertexEQ(node->vertex, vertex))
            {
                return true;
            }
            ++it;
        }
        return false;
    }

    virtual float weight(T from, T to) override
    {
        // TODO
        VertexNode *fromNode = getVertexNode(from);
        if (!fromNode)
        {
            throw VertexNotFoundException("Vertex " + vertex2str(from) + " not found.");
        }
        typename DLinkedList<Edge*>::Iterator edgeIt = fromNode->adList.begin();
        while (edgeIt != fromNode->adList.end()) {
            Edge* edge = *edgeIt;
            if (vertexEQ(edge->to->vertex, to)) {
                return edge->weight;
            }
            edgeIt++;
        }
        throw EdgeNotFoundException("Edge from " + vertex2str(from) + " to " + vertex2str(to) + " not found.");
        
        Edge *edge = fromNode->getEdge(getVertexNode(to));
        if (!edge)
        {
            throw EdgeNotFoundException("Edge from " + vertex2str(from) + " to " + vertex2str(to) + " not found.");
        }
        return edge->weight;
    }

    virtual DLinkedList<T> getOutwardEdges(T from) override
    {
        // TODO
        VertexNode *fromNode = getVertexNode(from);
        if (!fromNode)
        {
            throw VertexNotFoundException("Vertex " + vertex2str(from) + " not found.");
        }
        // DLinkedList<T> outwardEdges;
        // typename DLinkedList<Edge*>::Iterator edgeIt = fromNode->adList.begin();
        // while (edgeIt != fromNode->adList.end()) {
        //     Edge* edge = *edgeIt;
        //     outwardEdges.add(edge->to->vertex);
        //     edgeIt++;
        // }
        // return outwardEdges;
        return fromNode->getOutwardEdges();
    }

    virtual DLinkedList<T> getInwardEdges(T to) override
    {
        cout << "Getting inward edges for vertex: " << to << endl;

        VertexNode *toNode = getVertexNode(to);
        if (!toNode)
        {
            cerr << "Error: Vertex " << vertex2str(to) << " not found in graph." << endl;
            throw VertexNotFoundException("Vertex " + vertex2str(to) + " not found.");
        }

        DLinkedList<T> inwardEdges;
        typename DLinkedList<VertexNode *>::Iterator nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *fromNode = *nodeIt;
            typename DLinkedList<Edge *>::Iterator edgeIt = fromNode->adList.begin();
            while (edgeIt != fromNode->adList.end())
            {
                Edge *edge = *edgeIt;
                if (edge->to == toNode)
                {
                    inwardEdges.add(fromNode->vertex); // Thêm đỉnh nguồn của cạnh vào danh sách
                }
                edgeIt++;
            }
            nodeIt++;
        }
        return inwardEdges;
    }

    virtual int size() override
    {
        // TODO
        return nodeList.size();
    }

    virtual bool empty() override
    {
        // TODO
        return nodeList.empty();
    };

    virtual void clear() override
    {
        // TODO
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while (it != nodeList.end())
        {
            VertexNode *node = *it;
            typename DLinkedList<Edge *>::Iterator edge_It = node->adList.begin();
            while (edge_It != node->adList.end())
            {
                Edge *edge = *edge_It;
                delete edge;
                edge_It++;
            }
            delete node;
            it++;
        }
        nodeList.clear();
    }

    virtual int inDegree(T vertex) override
    {
        // TODO
        VertexNode *vertexNode = getVertexNode(vertex);
        if (!vertexNode)
        {
            throw VertexNotFoundException("Vertex not found");
        }
        return vertexNode->inDegree();
    }

    virtual int outDegree(T vertex) override
    {
        // TODO
        VertexNode *vertexNode = getVertexNode(vertex);
        if (!vertexNode)
        {
            throw VertexNotFoundException("Vertex not found");
        }
        return vertexNode->outDegree();
    }

    virtual DLinkedList<T> vertices() override
    {
        DLinkedList<T> vertexList;
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while (it != nodeList.end())
        {
            VertexNode *node = *it;
            vertexList.add(node->getVertex());
            ++it;
        }
        return vertexList;
    }

    virtual bool connected(T from, T to) override
    {
        // TODO
        VertexNode *fromNode = getVertexNode(from);
        VertexNode *toNode = getVertexNode(to);
        if (!fromNode)
        {
            throw VertexNotFoundException("Vertex " + vertex2str(from) + " not found.");
        }
        if (!toNode)
        {
            throw VertexNotFoundException("Vertex " + vertex2str(to) + " not found.");
        }
        typename DLinkedList<Edge *>::Iterator edgeIt = fromNode->adList.begin();
        while (edgeIt != fromNode->adList.end())
        {
            Edge *edge = *edgeIt;
            if (edge->to == toNode)
            {
                return true;
            }
            ++edgeIt;
        }
        return false;
    }

    void println()
    {
        cout << this->toString() << endl;
    }
    virtual string toString() override
    {
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode *>::Iterator nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;

        os << left << setw(12) << "Edges:" << endl;

        nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;

            typename DLinkedList<Edge *>::Iterator edgeIt = node->adList.begin();
            while (edgeIt != node->adList.end())
            {
                Edge *edge = *edgeIt;
                os << edge->toString() << endl;

                edgeIt++;
            }

            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }

    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it;
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // BEGIN of VertexNode
    class VertexNode
    {
    private:
        template <class U>
        friend class UGraphModel; // UPDATED: added
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge *> adList;
        friend class Edge;
        friend class AbstractGraph;

        bool (*vertexEQ)(T &, T &);
        string (*vertex2str)(T &);

    public:
        VertexNode() : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ) {}
        VertexNode(T vertex, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
            : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ)
        {
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }
        T &getVertex()
        {
            return vertex;
        }

        void connect(VertexNode *to, float weight = 0)
        {
            // TODO
            Edge *new_Edge = new Edge(this, to, weight);
            adList.add(new_Edge);
            this->outDegree_++;
            to->inDegree_++;
        }

        DLinkedList<T> getOutwardEdges()
        {
            // TODO
            DLinkedList<T> outwardEdges;
            typename DLinkedList<Edge *>::Iterator edgeIt = adList.begin();
            while (edgeIt != adList.end())
            {
                Edge *edge = *edgeIt;
                outwardEdges.add(edge->to->vertex);
                edgeIt++;
            }
            return outwardEdges;
        }

        Edge *getEdge(VertexNode *to)
        {
            // TODO
            typename DLinkedList<Edge *>::Iterator edgeIt = adList.begin();
            while (edgeIt != adList.end())
            {
                Edge *edge = *edgeIt;
                if (edge->to == to)
                {
                    return edge;
                }
                edgeIt++;
            }
            return nullptr;
        }

        bool equals(VertexNode *node)
        {
            // TODO
            return vertexEQ(this->vertex, node->vertex);
        }

        void removeTo(VertexNode *to)
        {
            typename DLinkedList<Edge *>::Iterator edgeIt = adList.begin();

            while (edgeIt != adList.end())
            {
                Edge *edge = *edgeIt;

                if (edge->to == to)
                {
                    // Safely remove and delete the edge
                    adList.removeItem(edge);
                    delete edge;
                    outDegree_ = max(0, outDegree_ - 1);

                    edgeIt = adList.begin();
                    inDegree_ = std::max(0, inDegree_ - 1);
                }
                else
                {
                    edgeIt++; // Move to the next edge
                }
            }
        }

        int inDegree()
        {
            // TODO
            return this->inDegree_;
        }
        int outDegree()
        {
            // TODO
            return this->outDegree_;
        }
        string toString()
        {
            stringstream os;
            os << "V("
               << this->vertex << ", "
               << "in: " << this->inDegree_ << ", "
               << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
    // END of VertexNode

    // BEGIN of Edge
    class Edge
    {
    private:
        VertexNode *from;
        VertexNode *to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;

    public:
        Edge() {}
        Edge(VertexNode *from, VertexNode *to, float weight = 0)
        {
            this->from = from;
            this->to = to;
            this->weight = weight;
        }

        bool equals(Edge *edge)
        {
            // TODO
            return (this->from->equals(edge->from) &&
                    this->to->equals(edge->to) &&
                    this->weight == edge->weight);
        }

        static bool edgeEQ(Edge *&edge1, Edge *&edge2)
        {
            return edge1->equals(edge2);
        }
        string toString()
        {
            stringstream os;
            os << "E("
               << this->from->vertex
               << ","
               << this->to->vertex
               << ","
               << this->weight
               << ")";
            return os.str();
        }
    };
    // END of Edge

    // BEGIN of Iterator
public:
    class Iterator
    {
    private:
        typename DLinkedList<VertexNode *>::Iterator nodeIt;

    public:
        Iterator(AbstractGraph<T> *pGraph = 0, bool begin = true)
        {
            if (begin)
            {
                if (pGraph != 0)
                    nodeIt = pGraph->nodeList.begin();
            }
            else
            {
                if (pGraph != 0)
                    nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator &operator=(const Iterator &iterator)
        {
            this->nodeIt = iterator.nodeIt;
            return *this;
        }

        T &operator*()
        {
            return (*nodeIt)->vertex;
        }

        bool operator!=(const Iterator &iterator)
        {
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            nodeIt++;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    // END of Iterator
};

#endif /* ABSTRACTGRAPH_H */

// // VertexNode* vertexNode = getVertexNode(vertex);
// if (!vertexNode) {
//     throw VertexNotFoundException("Vertex not found");
// }
// int count = 0;
// // Duyệt qua tất cả các đỉnh trong nodeList
// typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
// while (it != nodeList.end()) {
//     VertexNode* currentNode = *it;
//     // Duyệt qua danh sách các cạnh của mỗi đỉnh và đếm số cạnh đi ra
//     typename DLinkedList<Edge*>::Iterator edgeIt = currentNode->adList.begin();
//     while (edgeIt != currentNode->adList.end()) {
//         Edge* edge = *edgeIt;
//         if (edge->from == vertexNode) {
//             count++;
//         }
//         edgeIt++;
//     }
//     it++;
// }
// return count;