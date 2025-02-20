/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T>
{
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0) : DLinkedList<T>(removeData, itemEQ) {

                                        };

    DLinkedListSE(const DLinkedList<T> &list)
    {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0)
    {
        if (this->count <= 1)
            return;

        // Create dynamic array to store elements
        T *arr = new T[this->count];
        typename DLinkedList<T>::Node *current = this->head->next;
        int index = 0;

        // Copy data to array
        while (current != this->tail && index < this->count)
        {
            arr[index++] = current->data;
            current = current->next;
        }

        // Selection sort with correct comparator logic
        for (int i = 0; i < this->count - 1; i++)
        {
            int minIdx = i;
            for (int j = i + 1; j < this->count; j++)
            {
                // Use compare function correctly
                if (compare(arr[j], arr[minIdx], comparator) < 0)
                {
                    minIdx = j;
                }
            }
            if (minIdx != i)
            {
                T temp = arr[i];
                arr[i] = arr[minIdx];
                arr[minIdx] = temp;
            }
        }

        // Update linked list with sorted array
        current = this->head->next;
        for (int i = 0; i < this->count; i++)
        {
            current->data = arr[i];
            current = current->next;
        }

        delete[] arr;
    }

    

protected:
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0)
    {
        if (comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if (lhs < rhs)
                return -1;
            else if (lhs > rhs)
                return +1;
            else
                return 0;
        }
    }
};
#endif /* DLINKEDLISTSE_H */
