//
//  simd.cpp
//  OptTest
//
//  Created by Yongsik Im on 7/14/25.
//
#include "simd.h"

void multiply_matrix_simd(const matrix& lhs, const matrix& rhs, matrix& result)
{
    assert(lhs.col == rhs.row);
    size_t len = lhs.col;
    size_t row = lhs.row;
    size_t col = rhs.col;
#if USE_SSE
    // SSE는 128비트 레지스터를 사용하며, 32비트 float 4개를 동시에 처리 가능
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; j += 4)
        {                                    // 한 번에 4개의 열을 처리
            __m128 c_vec = _mm_setzero_ps(); // 합계를 저장할 벡터 레지스터를 0으로 초기화
            for (int k = 0; k < col; ++k)
            {
                // A[i][k] 값을 4개의 float 벡터로 브로드캐스트
                __m128 a_vec = _mm_set1_ps(&(lhs[i * row + k]));
                // B[k][j]부터 4개의 float를 벡터 레지스터로 로드
                __m128 b_vec = _mm_loadu_ps(&(rhs[k * row + j]));
                
                // 곱셈-누적 연산: c_vec = (a_vec * b_vec) + c_vec
                c_vec = _mm_add_ps(c_vec, _mm_mul_ps(a_vec, b_vec));
            }
            // 최종 결과를 메모리(C[i][j])에 저장
            _mm_storeu_ps(&(result[i * row + j]), c_vec);
        }
    }
#elif USE_NEON
    // NEON (ARM) 버전의 아이디어 스케치 (컴파일 환경에 따라 수정 필요)
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; j += 4)
        {
            float32x4_t c_vec = vmovq_n_f32(0.0f); // 0으로 초기화
            for (int k = 0; k < len; ++k)
            {
                
                float32x4_t a_vec = vld1q_dup_f32(&(lhs.data[i * lhs.row + k])); // 값 하나를 복제하여 로드
                float32x4_t b_vec = vld1q_f32(&rhs.data[k * rhs.row + j]);
                // fast multiply add
                c_vec             = vfmaq_f32(c_vec, a_vec, b_vec);
            }
            vst1q_f32(&result.data[i * row + j], c_vec); // 결과 저장
        }
    }
#endif
}

void multiply_matrix_scalar(const matrix& lhs, const matrix& rhs, matrix& result)
{
    assert(lhs.col == rhs.row);
    size_t len = lhs.col;
    size_t row = lhs.row;
    size_t col = rhs.col;
    
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            float sum = 0.0f;
            for (int k = 0; k < len; ++k)
            {
                sum += lhs.data[i * lhs.row + k] * rhs.data[k * rhs.row + j];
            }
            result.data[i * row + j] = sum;
        }
    }
}

void multiply_many_matrix_simd(const std::vector<matrix>& lhs, const std::vector<matrix>& rhs, std::vector<matrix>& result)
{
    assert(lhs.size() == rhs.size());
    size_t count = lhs.size();
    result.resize(count);
    
    for(size_t i = 0; i < count; i++)
    {
        multiply_matrix_simd(lhs[i], rhs[i], result[i]);
    }
}

void multiply_many_matrix_scalar(const std::vector<matrix>& lhs, const std::vector<matrix>& rhs, std::vector<matrix>& result)
{
    assert(lhs.size() == rhs.size());
    size_t count = lhs.size();
    result.resize(count);
    for(size_t i = 0; i < count; i++)
    {
        multiply_matrix_scalar(lhs[i], rhs[i], result[i]);
    }
    
}

