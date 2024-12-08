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

        // Tạo mảng động để lưu các phần tử
        T *arr = new T[this->count];
        typename DLinkedList<T>::Node *current = this->head;
        int index = 0;

        // Copy dữ liệu vào mảng
        while (current != nullptr && index < this->count)
        {
            arr[index++] = current->data;
            current = current->next;
        }

        // Sắp xếp mảng bằng selection sort
        for (int i = 0; i < this->count - 1; i++)
        {
            int minIdx = i;
            for (int j = i + 1; j < this->count; j++)
            {
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

        // Cập nhật lại linked list
        current = this->head;
        for (int i = 0; i < this->count; i++)
        {
            current->data = arr[i];
            current = current->next;
        }

        delete[] arr;
    }

    void reverseSort(int (*comparator)(T &, T &) = 0)
    {
        this->sort(comparator);

        // Đảo ngược danh sách liên kết
        if (this->count <= 1)
            return;

        typename DLinkedList<T>::Node *left = this->head;
        typename DLinkedList<T>::Node *right = this->head;

        // Tìm node cuối cùng
        while (right->next != nullptr)
        {
            right = right->next;
        }

        // Đảo ngược liên kết giữa các node
        while (left != right && left->prev != right)
        {
            T temp = left->data;
            left->data = right->data;
            right->data = temp;

            left = left->next;
            right = right->prev;
        }
    }

    // typename DLinkedList<T>::Node *mergeSort(typename DLinkedList<T>::Node *head, int (*comparator)(T &, T &))
    // {
    //     if (head == nullptr || head->next == nullptr)
    //         return head;

    //     // Tìm điểm giữa
    //     typename DLinkedList<T>::Node *middle = getMiddle(head);
    //     typename DLinkedList<T>::Node *secondHalf = middle->next;

    //     // Ngắt liên kết giữa hai nửa
    //     middle->next = nullptr;
    //     if (secondHalf)
    //         secondHalf->prev = nullptr;

    //     // Đệ quy sắp xếp hai nửa
    //     typename DLinkedList<T>::Node *left = mergeSort(head, comparator);
    //     typename DLinkedList<T>::Node *right = mergeSort(secondHalf, comparator);

    //     // Merge hai nửa đã sắp xếp
    //     return merge(left, right, comparator);
    // }

    // typename DLinkedList<T>::Node *merge(typename DLinkedList<T>::Node *left,
    //                                      typename DLinkedList<T>::Node *right,
    //                                      int (*comparator)(T &, T &))
    // {
    //     if (left == nullptr)
    //         return right;
    //     if (right == nullptr)
    //         return left;

    //     typename DLinkedList<T>::Node *result = nullptr;

    //     // So sánh và merge
    //     if (compare(left->data, right->data, comparator) <= 0)
    //     {
    //         result = left;
    //         result->next = merge(left->next, right, comparator);
    //     }
    //     else
    //     {
    //         result = right;
    //         result->next = merge(left, right->next, comparator);
    //     }

    //     // Cập nhật con trỏ prev cho node kế tiếp
    //     if (result->next != nullptr)
    //     {
    //         result->next->prev = result;
    //     }

    //     return result;
    // }

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
