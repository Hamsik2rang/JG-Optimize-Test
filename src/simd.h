//
//  simd.h
//  OptTest
//
//  Created by Yongsik Im on 7/14/25.
//
#pragma once
#ifndef __SIMD_H__
#define __SIMD_H__

#include "Predefine.h"
#include "common.h"

void multiply_matrix_simd(const matrix& lhs, const matrix& rhs, matrix& result);
void multiply_matrix_scalar(const matrix& lhs, const matrix& rhs, matrix& result);

void multiply_many_matrix_simd(const std::vector<matrix>& lhs, const std::vector<matrix>& rhs, std::vector<matrix>& result);
void multiply_many_matrix_scalar(const std::vector<matrix>& lhs, const std::vector<matrix>& rhs, std::vector<matrix>& result);

#endif
