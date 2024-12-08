/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    xt::xarray<double> Y = softmax(X, m_nAxis);
    this->m_aCached_Y = Y;
    return Y;
}

xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    xt::xarray<double> Y = this->m_aCached_Y;

    // Lambda function để tính gradient cho từng mẫu
    auto heloisme = [](const xt::xarray<double>& prob, const xt::xarray<double>& dy) {
        xt::xarray<double> diag_prob = xt::diag(prob);
        xt::xarray<double> prob_outer = xt::linalg::outer(prob, prob);
        xt::xarray<double> jacobian_matrix = diag_prob - prob_outer;
        return xt::linalg::dot(jacobian_matrix, dy);
    };

    // Khởi tạo mảng kết quả cho gradient với cùng kích thước batch và số lượng đặc trưng
    unsigned long batch_size = Y.shape(0);
    unsigned long feature_count = Y.shape(1);
    xt::xarray<double> grad_output = xt::zeros<double>({batch_size, feature_count});

    // Xử lý đặc biệt cho trường hợp chỉ có một mẫu
    if (batch_size == 1) {
        auto grad_single = heloisme(xt::row(Y, 0), xt::row(DY, 0));
        xt::row(grad_output, 0) = grad_single;
        return grad_output;
    }

    // Tính gradient cho mỗi mẫu trong batch
    #pragma omp parallel for if (batch_size > 100)
    for (unsigned long i = 0; i < batch_size; ++i) {
        auto sample_prob = xt::row(Y, i);
        auto sample_dy = xt::row(DY, i);

        auto sample_grad = heloisme(sample_prob, sample_dy);

        xt::row(grad_output, i) = sample_grad;
    }
    return grad_output;
}
string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
