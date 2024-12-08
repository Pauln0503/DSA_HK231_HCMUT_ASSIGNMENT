/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"

#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy& orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t) {
    this->m_aCached_Ypred = X;
    this->m_aYtarget = t;
    return cross_entropy(X,t, REDUCE_MEAN);
}

xt::xarray<double> CrossEntropy::backward() {
    int haha = m_aCached_Ypred.shape(0);
    const double EPSILON = 1e-7;
    xt::xarray<double> safe_preds = m_aCached_Ypred + EPSILON;  
    xt::xarray<double> gradient = -m_aYtarget / safe_preds;
    
    if (m_eReduction == REDUCE_MEAN) {
        gradient /= static_cast<double>(haha);
    }
    return gradient;
}
