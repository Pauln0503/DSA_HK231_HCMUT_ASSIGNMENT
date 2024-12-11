/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e) override;
    void add(int index, T e) override;
    T removeAt(int index) override;
    bool removeItem(T item, void (*removeItemData)(T) = 0) override;
    bool empty() override;
    int size() override;
    void clear() override;
    T &get(int index) override;
    int indexOf(T item) override;
    bool contains(T item) override;
    string toString(string (*item2str)(T &) = 0) override;
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };
    //////////////////////////////////////////////////////////////////////
    class BWDIterator
{
private:
    DLinkedList<T> *pList; // Con trỏ tới danh sách liên kết đôi
    Node *pNode;           // Con trỏ tới nút hiện tại trong danh sách

public:
    // Constructor: khởi tạo iterator
    BWDIterator(DLinkedList<T> *pList = 0, bool last = true)
    {
        if (last)
        {
            if (pList != nullptr && pList->tail != nullptr)
                this->pNode = pList->tail->prev;
            else
                pNode = nullptr;
        }
        else
        {
            if (pList != nullptr && pList->head != nullptr)
                this->pNode = pList->head;
            else
                pNode = nullptr;
        }

    }

    // Gán iterator này với một iterator khác
    BWDIterator &operator=(const BWDIterator &iterator)
    {
        this->pNode = iterator.pNode;
        this->pList = iterator.pList;
        return *this;
    }

    // Xóa phần tử hiện tại và di chuyển iterator lùi lại
    void remove(void (*removeItemData)(T) = 0)
    {
        if (pNode == nullptr)
        return;

    // Nếu là nút đầu tiên hoặc cuối cùng
    if (pNode == pList->head)
        pList->head = pNode->next;
    if (pNode == pList->tail)
        pList->tail = pNode->prev;

    pNode->prev->next = pNode->next;
    pNode->next->prev = pNode->prev;
    Node *pPrev = pNode->prev; // Lưu lại nút trước khi xóa
    if (removeItemData != nullptr)
        removeItemData(pNode->data);
    delete pNode;
    pNode = pPrev; 
    pList->count -= 1;
    }

    // Trả về dữ liệu của nút hiện tại
    T &operator*()
    {
        return pNode->data;
    }

    // So sánh hai iterator có khác nhau không
    bool operator!=(const BWDIterator &iterator)
    {
        return pNode != iterator.pNode;
    }

    // Toán tử tiền tố -- (di chuyển iterator về trước)
    BWDIterator operator++(int)
    {
        BWDIterator iterator = *this;
        if (pNode != nullptr) {
            pNode = pNode->prev;
        }
        return iterator;
    }

    // Fix pre-increment operator 
    BWDIterator &operator++()
    {
        if (pNode != nullptr) {
            pNode = pNode->prev;
        }
        return *this;
    }

    // Fix post-decrement operator
    BWDIterator operator--(int)
    {
        BWDIterator iterator = *this;
        if (pNode != nullptr) {
            pNode = pNode->prev;
        }
        return iterator;
    }

    // Fix pre-decrement operator
    BWDIterator &operator--()
    {
        if (pNode != nullptr) {
            pNode = pNode->prev;
        }
        return *this;
    }
};


    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
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
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO
    head = new Node();
    tail = new Node(); 
    head->next = tail;
    tail->prev = head;
    count = 0;
    this->deleteUserData = deleteUserData; 
    this->itemEqual = itemEqual;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    count = 0;

    Node *Cur = list.head->next;
    while (Cur != list.tail) {
        add(Cur->data);
        Cur = Cur->next;
    }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    this->count = 0;
    this->head = new Node();
    this->head->next = this->tail;
    this->tail = new Node();
    this->tail->prev = this->head;
    Node *Cur = list.head->next;
    while (Cur != list.tail)
    {
        add(Cur->data);
        Cur = Cur->next;
    }
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // Node *cur = head->next;
    // while (cur != tail) {
    //     Node *nextNode = cur->next;
    //     delete cur;
    //     cur = nextNode;
    // }
    clear();
    delete tail;  // tail được xóa sau khi giải phóng các nút
    delete head;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    Node* newnode = new Node(e, nullptr, nullptr);

    if (count == 0) {
        head->next = newnode;
        tail->prev = newnode;
        newnode->prev = head;
        newnode->next = tail;
    } else {
        Node* last = tail->prev;
        last->next = newnode;
        newnode->prev = last;
        newnode->next = tail;
        tail->prev = newnode;
    }
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    
    if (index < 0 || index > count) {
        throw std::out_of_range("Index is out of range!");
    }

    Node *newNode = new Node(e);
    Node *cur = head;

    if (index == 0) {
        newNode->next = head->next;
        newNode->prev = head;
        if (head->next != nullptr) {
            head->next->prev = newNode;
        }
        head->next = newNode;
        if (count == 0) {
            tail->prev = newNode;
        }
    }
    
    else if (index == count) {
        newNode->prev = tail->prev;
        newNode->next = tail;
        if (tail->prev != nullptr) {
            tail->prev->next = newNode;
        }
        tail->prev = newNode;
    }
    else {
        Node* targetNode = getPreviousNodeOf(index);
        newNode->next = targetNode;
        newNode->prev = targetNode->prev;
        targetNode->prev->next = newNode;
        targetNode->prev = newNode;
    } //or ( cái trên dùng hàm kia hỗ trợ, dứoi thì ko có)
    // else {
    //     for (int i = 0; i < index; i++) {
    //         cur = cur->next;
    //     }
    //     newNode->next = cur->next;
    //     newNode->prev = cur;
    //     cur->next->prev = newNode;
    //     cur->next = newNode;
    // }

    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    // TODO
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index out of range");
    }

    Node* check;
    if (index < count / 2) {
        check = head->next;
        for (int i = 0; i < index; i++) {
            check = check->next;
        }
    } else {
        check = tail->prev;
        for (int i = count - 1; i > index; i--) {
            check = check->prev;
        }
    }
    return check;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    // if ((count < 0) || (index >= count))
    // {
    //     throw out_of_range("Index is out of range");
    // }
    // Node *temp = head->next;
    // for (int i = 0; i < index; i++)
    // {
    //     temp = temp->next;
    // }
    // T result = temp->data;
    // temp->next->prev = temp->prev; // tro next cua con truoc cur toi prev con sau cuar cur
    // temp->prev->next = temp->next; // nguoc lai nhu tren
    // delete temp;
    // count--;
    // return result;

    if (index < 0 || index >= count) {
        throw out_of_range("Index is out of range!");
    }

    Node* temp = head->next;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    }

    T result = temp->data;
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    delete temp;
    count--;

    return result;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    if (count == 0)
    {
        return true;
    }
    return false;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    Node *Cur = head->next;
    while (Cur != tail)
    {
        Node *nextnode = Cur->next;
        delete Cur;
        Cur = nextnode;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
    
    
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if ((index < 0) || (index >= count))
    {
        throw out_of_range("Index is out of range!");
    }

    Node *Cur = head->next;
    for (int i = 0; i < index; i++)
    {
        Cur = Cur->next;
    }
    return Cur->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    Node *find = head->next;
    int counting = 0;
    while (find != tail)
    {
        if (itemEqual != nullptr)
        {
            if (itemEqual(find->data, item))
            {
                return counting;
            }
        }
        else {
            if (find->data == item){
                return counting;
            }
        }

        find = find->next;
        counting++;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    Node *check = head->next;
    while (check != tail)
    {
        if (itemEqual ? itemEqual(check->data, item) : (check->data == item))
        {
            if (removeItemData != nullptr)
            {
                removeItemData(check->data);
            }
            check->prev->next = check->next;
            check->next->prev = check->prev;
            delete check;
            count--;
            return true;
        }
        check = check->next;
    }
    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    if (indexOf(item) != -1)
    {
        return true;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO

    string final = "[";
    Node *current = head->next;

    while (current != tail) {
        if (item2str != nullptr) {
            final += item2str(current->data);
        } else {
            if constexpr (std::is_arithmetic<T>::value || std::is_convertible<T, std::string>::value) {
                std::ostringstream stream;
                stream << current->data; 
                string number = stream.str();
                size_t dot_pos = number.find('.');
                if (dot_pos != std::string::npos) {
                    size_t end_pos = dot_pos + 7; 
                    if (end_pos < number.size()) {
                        number = number.substr(0, end_pos);
                    }

                    // Xóa các số 0 dư
                    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
                    if (number.back() == '.') {
                        number.pop_back(); 
                    }
                }

                final += number;
            } else {
                throw std::invalid_argument("error");
            }
        }
        current = current->next;
        if (current != tail) {
            final += ", ";
        }
    }
    final += "]";
    return final;
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    // TODO

    // removeInternalData(); // chac la co dung nhi

    Node *head = new Node();
    Node *tail = new Node();
    head->next = tail;
    tail->prev = head;

    Node *copyy = list.head->next;
    while (copyy != list.tail)
    {
        add(copyy->data);
        copyy = copyy->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO
    Node *Cur = head->next;
    while (Cur != tail)
    {
        // lamj do
        Node *Next = Cur->next;

        if (deleteUserData != nullptr)
        {
            deleteUserData(Cur->data);
        }
        delete Cur;
        Cur = Next;
    }
    delete head;
    delete tail;

    count = 0;
    head = nullptr;
    tail = nullptr;
    
}

#endif /* DLINKEDLIST_H */
