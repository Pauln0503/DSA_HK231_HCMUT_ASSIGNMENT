/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator;
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;  
    //int* indices;
    xt::xarray<unsigned long> indices;
    int m_seed;
    
    /*TODO: add more member variables to support the iteration*/
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false, int seed=-1){
        /*TODO: Add your code to do the initialization */
        // initialize
       this->ptr_dataset= ptr_dataset;
       this-> batch_size= batch_size;
       this->shuffle= shuffle;
       this->drop_last= drop_last;
       this->m_seed = seed; 
       this->indices = xt::arange<unsigned long>( this->ptr_dataset->len());

                 
        // Nếu shuffle = true, xáo trộn chỉ số của dữ liệu
        if (this->shuffle) {
            if (m_seed >= 0) {
                xt::random::seed(m_seed); // Chỉ gọi seed nếu seed >= 0
                xt::random::shuffle(indices);
            }
            //auto engine = xt::random::default_engine_type(m_seed);
            // auto engine = (m_seed < 0) ? xt::random::default_engine_type() : xt::random::default_engine_type(m_seed);
            
            // Xáo trộn chỉ số
            else{
            xt::random::shuffle(indices);}
        }
    }
    virtual ~DataLoader(){ }
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    // int get_total_batch() {
    //     int dataset_size = ptr_dataset->len();
    //     int total_batches = dataset_size / batch_size;
        
    //     // Nếu `drop_last` là false và không chia hết, thêm 1 batch nữa
    //     if (!drop_last && (dataset_size % batch_size != 0)) {
    //         total_batches += 1;
    //     }

    //     return total_batches;
    // }
    // int get_sample_count()
    // {return ptr_dataset->len();}

    

    Iterator begin() {
        if (batch_size > ptr_dataset->len()) {
        return end(); 
    }
        return Iterator(this, 0);
    }

    Iterator end() {
        int dataset_size = ptr_dataset->len();
        int total_batches = (dataset_size < batch_size) ? 1 : (dataset_size  ) / batch_size ;
        if ( ptr_dataset->len() < batch_size && total_batches == 1 && drop_last){
            total_batches = 0; 
        }

    // Trả về iterator ở batch cuối cùng
    return Iterator(this, total_batches);
    }


    Batch<DType, LType> get_batch(int batch_index) {
        //cout << batch_index;
    int dataset_size = ptr_dataset->len();
    int start = batch_index * batch_size;

    // Tính toán end, đảm bảo không vượt quá kích thước dataset
     int end = xt::amin(xt::xarray<int>({ (start + batch_size), (int)ptr_dataset->len()}))(0);
     end = (ptr_dataset->len() > batch_size && !drop_last && batch_index == (dataset_size / batch_size-1) ) ? ptr_dataset->len() : (start + batch_size) ; 
     
    // cout << end << endl;

    // Tính kích thước batch thực tế
    int actual_batch_size = (drop_last ||  batch_index != (dataset_size / batch_size-1)) ? batch_size : (batch_size + (dataset_size % batch_size));
    if (dataset_size < batch_size ) { 
        end = dataset_size;
        actual_batch_size = dataset_size; 
    }
    
    // Khởi tạo kích thước của batch_data và batch_label
    //if (this->ptr_dataset->len() >= actual_batch_size && batch_index == (dataset_size / batch_size) - 1 ) actual_batch_size = this->ptr_dataset->len();
    
    auto batch_data_shape = this->ptr_dataset->get_data_shape();
    batch_data_shape[0] = actual_batch_size;

    
    // auto batch_label_shape = this->ptr_dataset->get_label_shape();
    // batch_label_shape[0] = actual_batch_size; 

    xt::xarray<DType> batch_data = xt::empty<DType>(batch_data_shape);
    //xt::xarray<LType> batch_label = xt::empty<LType>(batch_label_shape);

    xt::xarray<LType> batch_label;
    if (ptr_dataset->has_label()) { // Kiểm tra xem dataset có nhãn không
        auto batch_label_shape = this->ptr_dataset->get_label_shape();
        batch_label_shape[0] = actual_batch_size; 
        batch_label = xt::empty<LType>(batch_label_shape);
    }
    for (int i = start; i < end; ++i) {
        
        DataLabel<DType, LType> item = ptr_dataset->getitem(indices[i]);
        try {
        xt::view(batch_data, i - start, xt::all()) = item.getData();
        }
        catch (const std::bad_array_new_length& e) {
        
    }
    if (ptr_dataset->has_label()){
        try {
        xt::view(batch_label, i - start, xt::all()) = item.getLabel();
    }
    
    catch (const std::bad_array_new_length& e) {
        
    }
    }
    }
    return Batch<DType, LType>(batch_data, batch_label);
}




    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
    
    /*TODO: Add your code here to support iteration on batch*/
    class Iterator {
     private:
        DataLoader* loader;
        int batch_index;

    public:
        Iterator(DataLoader* loader, int batch_index) : loader(loader), batch_index(batch_index) {}
        Iterator& operator++() {
            ++batch_index;
            return *this;
    }

        bool operator!=(const Iterator& other) const {
            return batch_index != other.batch_index;
    }

        Batch<DType, LType> operator*() {
            return loader->get_batch(batch_index);
    }

    Iterator operator++(int) {
        Iterator temp = *this; 
        ++(*this);  
        return temp; 
    }
    Iterator &operator=(const Iterator &itera){
        batch_index = itera.batch_index;
        loader = itera.loader;
        return *this;
    }
};

    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

