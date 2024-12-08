/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template <typename DType, typename LType>
class DataLabel
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;

public:
    DataLabel(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label)
    {
    }
    xt::xarray<DType> getData() const { return data; }
    xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType>
class Batch
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;

public:
    Batch(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label)
    {
    }
    virtual ~Batch() {}
    xt::xarray<DType> &getData() { return data; }
    xt::xarray<LType> &getLabel() { return label; }
    
};

template <typename DType, typename LType>
class Dataset
{
private:
public:
    Dataset() {};
    virtual ~Dataset() {};

    virtual int len() = 0;
    virtual DataLabel<DType, LType> getitem(int index) = 0;
    virtual xt::svector<unsigned long> get_data_shape() = 0;
    virtual xt::svector<unsigned long> get_label_shape() = 0;
    bool has_label() {
        return get_label_shape().size() > 0;
    } ;
};

//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType>
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;

public:
    /* TensorDataset:
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
     */
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label): data(data), label(label)
    {
        /* TODO: your code is here for the initialization
         */
        // if (data.shape(0) != label.shape(0)){
        //     throw invalid_argument("Data and Label not equal");
        // }
        data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
        label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());
    }
    /* len():
     *  return the size of dimension 0
     */
    int len() override
    {
        /* TODO: your code is here to return the dataset's length
         */
        return data.shape(0);
    }

    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index) override
{
    // Kiểm tra chỉ số hợp lệ
    if (index < 0 || index >= data.shape()[0]) {
        throw std::out_of_range("Index is out of range!");
    }

    try {
        // Truy xuất dữ liệu bằng phương thức view để không sao chép dữ liệu
        auto DataItem = xt::view(data, index);

        xt::xarray<LType> LabelItem;
        if (label.shape().size() > 0) {
            // Nếu nhãn tồn tại, lấy nhãn tại chỉ số index
            LabelItem = xt::view(label, index);
        } else {
            // Nếu không có nhãn, tạo nhãn rỗng phù hợp với kích thước của DataItem
            xt::svector<unsigned long> empty_shape(DataItem.shape().begin() + 1, DataItem.shape().end());
            LabelItem = xt::zeros<LType>(empty_shape);
        }

        return DataLabel<DType, LType>(DataItem, LabelItem);
    } 
    catch (const std::bad_array_new_length& e) {
        return DataLabel<DType, LType>(this->data[index], this->label[index]);
    }
}


    xt::svector<unsigned long> get_data_shape() override
    {
        /* TODO: your code is here to return data_shape
         */
        return data_shape;
    }
    xt::svector<unsigned long> get_label_shape() override
    {
        /* TODO: your code is here to return label_shape
         */
        return label_shape;
    }

    // bool has_label()  {
    //     return label.size() > 0;  // Kiểm tra xem có nhãn hay không
    // }
    
};
//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class ImageFolderDataset : public Dataset<DType, LType>
{
private:
    xt::xarray<DType> data;              // Dữ liệu hình ảnh
    xt::xarray<LType> label;             // Nhãn tương ứng
    xt::svector<unsigned long> data_shape; // Kích thước của dữ liệu
    xt::svector<unsigned long> label_shape; // Kích thước của nhãn

public:
    // Constructor
    ImageFolderDataset(const xt::xarray<DType> &image_array, const xt::xarray<LType> &label_array)
        : data(image_array), label(label_array)
    {
        // if (data.shape().size() > 0)
        // {
        //     data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
        // }
        // label_shape = {label.shape(0)}; 
        data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
        label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());
    }

    // Trả về số lượng mẫu dữ liệu
    int len() override {
        return label.shape(0); // Độ dài của nhãn là số lượng mẫu
    }

    // Lấy một mẫu dữ liệu và nhãn theo chỉ số
    DataLabel<DType, LType> getitem(int index) override
{
    // Kiểm tra chỉ số hợp lệ
    if (index < 0 || index >= data.shape()[0]) {
        throw std::out_of_range("Index is out of range!");
    }

    try {
        // Truy xuất dữ liệu bằng phương thức view để không sao chép dữ liệu
        auto DataItem = xt::view(data, index);

        xt::xarray<LType> LabelItem;
        if (label.shape().size() > 0) {
            // Nếu nhãn tồn tại, lấy nhãn tại chỉ số index
            LabelItem = xt::view(label, index);
        } else {
            // Nếu không có nhãn, tạo nhãn rỗng phù hợp với kích thước của DataItem
            xt::svector<unsigned long> empty_shape(DataItem.shape().begin() + 1, DataItem.shape().end());
            LabelItem = xt::zeros<LType>(empty_shape);
        }

        return DataLabel<DType, LType>(DataItem, LabelItem);
    } 
    catch (const std::exception& e) {
       
    }
}
};

// Note: Shape.size() trả về số lượng chiều của mảng VÀ shape(i) trả về kích thước của chiều thứ i
// xt::xarray<DType> và xt::xarray<LType>:Đây là kiểu dữ liệu chính từ thư viện xtensor, đại diện cho mảng (tensor) đa chiều.
// data.shape(): return về kích thước của mảng theo các chiều dưới dạng một danh sách (vector). Ví dụ, nếu data là một ma trận 3x4, data.shape() sẽ trả về {3, 4}.
// xt::view(data, index): Sử dụng để lấy một phần của mảng mà không sao chép dữ liệu. view trả về một cái nhìn (view) của mảng, cho phép truy cập hoặc thao tác với một phần cụ thể của mảng.
// xt::svector<unsigned long>: Đây là một loại vector được sử dụng để lưu trữ kích thước của mảng. svector từ thư viện xtensor là một vector có thể chứa các giá trị kích thước của mảng.


#endif /* DATASET_H */
