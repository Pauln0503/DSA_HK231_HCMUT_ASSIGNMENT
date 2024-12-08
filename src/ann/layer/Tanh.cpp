/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Tanh.cpp
 * Author: ltsach
 * 
 * Created on September 1, 2024, 7:03 PM
 */

#include "layer/Tanh.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

Tanh::Tanh(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::Tanh(const Tanh& orig) {
    m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::~Tanh() {
}

xt::xarray<double> Tanh::forward(xt::xarray<double> X) {
    // YOUR CODE IS HERE
    // xt::xarray<double> A = xt::tanh(X);
    // m_aCached_Y = A;
    // return A;
     xt::xarray<double> A = xt::tanh(X);
     m_aCached_Y = A;
    return A;
}

xt::xarray<double> Tanh::backward(xt::xarray<double> DY) {
    // YOUR CODE IS HERE
    xt::xarray<double> DX = DY * (1 - m_aCached_Y * m_aCached_Y);
    return DX;
    // xt::xarray<double> tanh_squared = xt::square(m_aCached_Y);
    // xt::xarray<double> grad_input = DY * (1.0 - tanh_squared);
    // return grad_input;
    // xt::xarray<double> grad_input = DY * (1- pow(m_aCached_Y,2));
    // return grad_input;
}

string Tanh::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Tanh", this->getname());
    return desc;
}
