//
//  simd.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/14/25.
//
#pragma once
#ifndef __SIMD_H__
#define __SIMD_H__

#include "Predefine.h"
#include "common.h"

void set_random_matrix_elements(Matrix& m);

int multiply_matrix_simd(const Matrix& lhs, const Matrix& rhs, Matrix& result, bool is4x4 = false);
int multiply_matrix_scalar(const Matrix& lhs, const Matrix& rhs, Matrix& result, bool is4x4 = false);

int multiply_many_matrix_simd(const std::vector<Matrix>& lhs, const std::vector<Matrix>& rhs, std::vector<Matrix>& result);
int multiply_many_matrix_scalar(const std::vector<Matrix>& lhs, const std::vector<Matrix>& rhs, std::vector<Matrix>& result);

#endif
