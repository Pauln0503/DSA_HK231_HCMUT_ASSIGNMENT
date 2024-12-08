/*
 * File:   XArrayList.h
 */

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "list/IList.h"
#include <memory.h>
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class XArrayList : public IList<T>
{
public:
    class Iterator; // forward declaration

protected:
    T *data;                                 // dynamic array to store the list's items
    int capacity;                            // size of the dynamic array
    int count;                               // number of items stored in the array
    bool (*itemEqual)(T &lhs, T &rhs);       // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(XArrayList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    XArrayList(
        void (*deleteUserData)(XArrayList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0,
        int capacity = 10);
    XArrayList(const XArrayList<T> &list);
    XArrayList<T> &operator=(const XArrayList<T> &list);
    ~XArrayList();

    // Inherit from IList: BEGIN
    
    void add(T e) override;
    void add(int index, T e) override;
    T removeAt(int index) override;
    bool removeItem(T item, void (*removeItemData)(T) = 0) override;
    bool empty() override;
    int size() override;
    void clear() override;
    T& get(int index)override;
    int indexOf(T item) override;
    bool contains(T item) override;
    string toString(string (*item2str)(T&) = 0) override;

    // Inherit from IList: BEGIN

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }
    Iterator end()
    {
        return Iterator(this, count);
    }

    /** free:
     * if T is pointer type:
     *     pass THE address of method "free" to XArrayList<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  XArrayList<Point*> list(&XArrayList<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(XArrayList<T> *list)
    {
        typename XArrayList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

protected:
    void checkIndex(int index);     // check validity of index for accessing
    void ensureCapacity(int index); // auto-allocate if needed

    /** equals:
     * if T: primitive type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     * if T: object type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     *      Therefore, class of type T MUST override operator ==
     * if T: pointer type:
     *      indexOf, contains: will use function pointer "itemEqual"
     *      to: compare two items of T type
     *      Therefore:
     *      (1): must pass itemEqual to the constructor of XArrayList
     *      (2): must define a method for comparing
     *           the content pointed by two pointers of type T
     *          See: definition of "equals" of class Point for more detail
     */
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    void copyFrom(const XArrayList<T> &list);

    void removeInternalData();

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    // Iterator: BEGIN
    class Iterator
    {
    private:
        int cursor;
        XArrayList<T> *pList;

    public:
        Iterator(XArrayList<T> *pList = 0, int index = 0)
        {
            this->pList = pList;
            this->cursor = index;
        }
        Iterator &operator=(const Iterator &iterator)
        {
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            T item = pList->removeAt(cursor);
            if (removeItemData != 0)
                removeItemData(item);
            cursor -= 1; // MUST keep index of previous, for ++ later
        }

        T &operator*()
        {
            return pList->data[cursor];
        }
        bool operator!=(const Iterator &iterator)
        {
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            this->cursor++;
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
    // Iterator: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
XArrayList<T>::XArrayList(
    void (*deleteUserData)(XArrayList<T> *),
    bool (*itemEqual)(T &, T &),
    int capacity)
{
    data = new T[capacity];
    this->capacity = capacity;     
    this->count = 0;               
    this->deleteUserData = deleteUserData; 
    this->itemEqual = itemEqual;
}

template <class T>
void XArrayList<T>::copyFrom(const XArrayList<T> &list)
{
    /*
     * Copies the contents of another XArrayList into this list.
     * Initializes the list with the same capacity as the source list and copies all elements.
     * Also duplicates user-defined comparison and deletion functions, if applicable.
     */
    // TODO

    removeInternalData();
    count = list.count;
    capacity = list.capacity;
    T* data = new T[capacity];
    for(int i = 0; i < count ; i++){
        data[i] = list.data[i];
    }
    deleteUserData = list.deleteUserData;
    itemEqual = list.itemEqual;
}

template <class T>
void XArrayList<T>::removeInternalData()
{
    /*
     * Clears the internal data of the list by deleting the dynamic array and any user-defined data.
     * If a custom deletion function is provided, it is used to free the stored elements.
     * Finally, the dynamic array itself is deallocated from memory.
     */
    // TODO
    // if (deleteUserData)
    // {
    //     for (int i = 0; i < count; ++i)
    //     {
    //         // Xóa dữ liệu của từng phần tử thay vì truyền cả danh sách
    //         deleteUserData(this); 
    //     }
    // }

    // Xóa mảng động
    delete[] data;
    
    // Reset các giá trị
    data = nullptr;
    count = 0;
    capacity = 0;   
 }

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
    // TODO
    data = new T[capacity];
    for (int i = 0; i < count; i++) {
        data[i] = list.data[i];
    }
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
    removeInternalData(); // Giải phóng dữ liệu cũ
    count = list.count;
    capacity = list.capacity;
    data = new T[capacity]; // Sửa lỗi
    for (int i = 0; i < count; i++) {
        data[i] = list.data[i];
    }
    deleteUserData = list.deleteUserData;
    itemEqual = list.itemEqual;
}

template <class T>
XArrayList<T>::~XArrayList()
{
    removeInternalData();
}

// method add cuoi phan tu
template <class T>
void XArrayList<T>::add(T e)
{
   
    ensureCapacity(count);
    data[count++] = e;
}

// method add o index nao do
template <class T>
void XArrayList<T>::add(int index, T e)
{
    // if( (index < 0) || (index > count)){
    //     cout << "Index is out of range!" << endl;
    // }

    // if (count == capacity) {
    //     capacity *= 2;
    //     T* data1 = new T[capacity];
    //     for (int i = 0; i < count; ++i) {
    //         data1[i] = data[i];
    //     }
    //     delete[] data;
    //     data = data1;
    // }
    //     for (int i = count; i > index; --i) {
    //         data[i] = data[i - 1];
    //     }
    if (index != count) {
        checkIndex(index);  // Xử lý khi index nằm trong khoảng hợp lệ
    }
    ensureCapacity(count);  // Đảm bảo đủ dung lượng cho phần tử mới
    for (int i = count; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = e;
    count++;
}

template <class T>
T XArrayList<T>::removeAt(int index)
{
    if((index < 0) || (index > (count -1))){
        throw out_of_range("Index is out of range!");
    }

    T RemoveAt = data[index];

    for(int i = index; i < (count - 1) ; i++){
        data[i] = data[i+1];
    }
    count --;
    return RemoveAt;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
    for (int i = 0; i < count; i++) {
        if (equals(data[i], item, itemEqual)) {
            if (removeItemData != nullptr) {
                removeItemData(data[i]);
            }
            for (int j = i; j < count - 1; j++) {
                data[j] = data[j + 1];
            }
            count--;
            return true; 
        }
    }
    return false;
}

template <class T>
bool XArrayList<T>::empty()
{
    if(count == 0){
        return true;
    }
    else {
        return false;
    }
}

template <class T>
int XArrayList<T>::size()
{
    return count;
}

template <class T>
void XArrayList<T>::clear()
{           
// 1
    delete[] data;
    data = nullptr;
    count = 0;
    capacity =0;
// 2    
    // for(int i = 0; i< count;i++){
    //     data[i] = nullptr;
    // }
    // count = 0;
    // capacity =0;
}

template <class T>
T &XArrayList<T>::get(int index)
{
    if((index < 0) || (index > (count-1))){
        throw out_of_range("Index is out of range!");
    }
    return data[index];
}


template <class T>
int XArrayList<T>::indexOf(T item)
{
    for (int i = 0; i < count; i++) {
        if (equals(data[i], item, itemEqual)) {
            return i;
        }
    }
    return -1;
}


template <class T>
bool XArrayList<T>::contains(T item)
{
    return indexOf(item) != -1;
}


template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the array list into a string representation, formatting each element using a user-defined function.
     * If no function is provided, it uses the default string representation for each element.
     * Example: Given an array list with elements {1, 2, 3} and a function that converts integers to strings, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function pointer for converting items of type T to strings. If null, default to the string conversion of T.
     * @return A string representation of the array list with elements separated by commas and enclosed in square brackets.
     */

    ostringstream lukaku;

    lukaku << "["; 
    for (int i = 0; i < count; ++i) {
        if (item2str != nullptr) {
            lukaku << item2str(data[i]);
        } else {
        lukaku << data[i];
        }
        if (i < count - 1) {
            lukaku << ", ";
        }
    }
    lukaku << "]";  

    return lukaku.str(); 
}

//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////
template <class T>
void XArrayList<T>::checkIndex(int index)
{
    /**
     * Validates whether the given index is within the valid range of the list.
     * Throws an std::out_of_range exception if the index is negative or exceeds the number of elements.
     * Ensures safe access to the list's elements by preventing invalid index operations.
     */
    // TODO
    if( (index < 0) || (index > count)){
        throw out_of_range("Index is out of range!");
    }

}
template <class T>
void XArrayList<T>::ensureCapacity(int index)
{
    /**
     * Ensures that the list has enough capacity to accommodate the given index.
     * If the index is out of range, it throws an std::out_of_range exception. If the index exceeds the current capacity,
     * reallocates the internal array with increased capacity, copying the existing elements to the new array.
     * In case of memory allocation failure, catches std::bad_alloc.
     */
    // TODO
    if (index < 0)
    {
        throw out_of_range("Index is out of range!");
    }

    // Nếu dung lượng hiện tại không đủ
    if (index >= capacity)
    {
        try
        {
            int newCapacity = capacity * 2;
            if (newCapacity <= index)
            {
                newCapacity = index + 1;
            }

            // Cấp phát mảng mới với dung lượng lớn hơn
            T* newData = new T[newCapacity];
            
            // Sao chép dữ liệu từ mảng cũ sang mảng mới
            for (int i = 0; i < count; ++i)
            {
                newData[i] = data[i];
            }

            // Xóa mảng cũ
            delete[] data;

            // Cập nhật con trỏ và dung lượng
            data = newData;
            capacity = newCapacity;
        }
        catch (const bad_alloc&)
        {
            throw runtime_error("Memory allocation failed!");
        }
    }

}

#endif /* XARRAYLIST_H */
