//
//  simd.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/14/25.
//
#include "simd.h"

void set_random_matrix_elements(Matrix& m)
{
	for (int i = 0; i < m.row; i++)
	{
		for (int j = 0; j < m.col; j++)
		{
			m.At(i, j) = Random::NextFloat(-100.0f, 100.0f);
		}
	}
}

static Matrix transpose_matrix(const Matrix& src)
{
	Matrix t(src.col, src.row);
	for (int i = 0; i < src.row; i++)
	{
		for (int j = 0; j < src.col; j++)
		{
			t.At(j, i) = src.At(i, j);
		}
	}
	return t;
}

int multiply_matrix_simd(const Matrix& lhs, const Matrix& rhs, Matrix& result, bool is4x4)
{
	assert(lhs.col == rhs.row);
	size_t row = lhs.row;
	size_t len = lhs.col;
	size_t col = rhs.col;

	if (is4x4)
	{
#if USE_SSE
		for (int i = 0; i < 4; i++)
		{
			__m128 rowips = _mm_loadu_ps(lhs.ReadRef(i, 0));

			__m128 xxxx = _mm_shuffle_ps(rowips, rowips, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 yyyy = _mm_shuffle_ps(rowips, rowips, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 zzzz = _mm_shuffle_ps(rowips, rowips, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 wwww = _mm_shuffle_ps(rowips, rowips, _MM_SHUFFLE(3, 3, 3, 3));

			__m128 row0ps = _mm_loadu_ps(rhs.ReadRef(0, 0));
			__m128 row1ps = _mm_loadu_ps(rhs.ReadRef(1, 0));
			__m128 row2ps = _mm_loadu_ps(rhs.ReadRef(2, 0));
			__m128 row3ps = _mm_loadu_ps(rhs.ReadRef(3, 0));

			__m128 e0 = _mm_mul_ps(xxxx, row0ps);
			__m128 e1 = _mm_mul_ps(yyyy, row1ps);
			__m128 e2 = _mm_mul_ps(zzzz, row2ps);
			__m128 e3 = _mm_mul_ps(wwww, row3ps);

			__m128 e01 = _mm_add_ps(e0, e1);
			__m128 e02 = _mm_add_ps(e2, e3);

			_mm_storeu_ps(result.WriteRef(i, 0), _mm_add_ps(e01, e02));
		}
#elif USE_NEON
		for (int i = 0; i < 4; ++i)
		{
			// 1. this(LHS)의 i번째 행을 NEON 레지스터로 로드합니다.
			float32x4_t lhs_row = vld1q_f32(row[i].elem);

			// 2. rhs(RHS)의 각 행을 로드합니다.
			float32x4_t rhs_row0 = vld1q_f32(rhs.row[0].elem);
			float32x4_t rhs_row1 = vld1q_f32(rhs.row[1].elem);
			float32x4_t rhs_row2 = vld1q_f32(rhs.row[2].elem);
			float32x4_t rhs_row3 = vld1q_f32(rhs.row[3].elem);

			// 3. FMA 연산을 사용하여 한 번에 계산합니다.
			// res = (lhs.x * rhs_row0)
			float32x4_t result_row = vmulq_laneq_f32(rhs_row0, lhs_row, 0);
			// res += (lhs.y * rhs_row1)
			result_row = vfmaq_laneq_f32(result_row, rhs_row1, lhs_row, 1);
			// res += (lhs.z * rhs_row2)
			result_row = vfmaq_laneq_f32(result_row, rhs_row2, lhs_row, 2);
			// res += (lhs.w * rhs_row3)
			result_row = vfmaq_laneq_f32(result_row, rhs_row3, lhs_row, 3);

			// 4. 최종 결과를 result 행렬에 저장합니다.
			vst1q_f32(result.row[i].elem, result_row);
		}
#endif
	}
	else
	{
		Matrix rhsT = transpose_matrix(rhs);

		// SSE는 128비트 레지스터를 사용하며, 32비트 float 4개를 동시에 처리 가능
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				float sumElem = 0.0f;
				int k = 0;
#if USE_SSE
				__m128 sumVec = _mm_setzero_ps();
				for (; k + 3 < len; k += 4)
				{
					__m128 lhsVec = _mm_loadu_ps(lhs.ReadRef(i, k));
					__m128 rhsTVec = _mm_loadu_ps(rhsT.ReadRef(j, k));
					sumVec = _mm_add_ps(sumVec, _mm_mul_ps(lhsVec, rhsTVec));
				}
				// sumVec(01, 23, 01, 23)
				sumVec = _mm_hadd_ps(sumVec, sumVec);
				// sumVec(0123, 0123, 0123, 0123)
				sumVec = _mm_hadd_ps(sumVec, sumVec);
				sumElem = _mm_cvtss_f32(sumVec);
#elif USE_NEON
				float32x4_t sumVec = vmovq_n_f32(0.0f);
				for (; k + 3 < len; k += 4)
				{
					float32x4_t lhsVec = vld1q_f32(lhs.ReadRef(i, k));
					float32x4_t rhsTVec = vld1q_f32(rhsT.ReadRef(j, k));
					sumVec = vfmaq_f32(sumVec, lhsVec, rhsTVec);
				}
				// Horizontal add (in AArch64)
				sumElem = vaddvq_f32(sumVec);
#endif
				// 남은 스칼라 값들 처리 (N이 4의 배수가 아닐 경우)
				for (; k < len; ++k)
				{
					sumElem += lhs.At(i, k) * rhsT.At(j, k);
				}
				result.At(i, j) = sumElem;
			}
		}
	}
	
	return 1;
}

int multiply_matrix_scalar(const Matrix& lhs, const Matrix& rhs, Matrix& result, bool is4x4)
{
	assert(lhs.col == rhs.row);
	size_t len = lhs.col;
	size_t row = lhs.row;
	size_t col = rhs.col;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			float sum = 0.0f;
			for (int k = 0; k < len; k++)
			{
				sum += lhs.At(i, k) * rhs.At(k, j);
			}
			result.At(i, j) = sum;
		}
	}
	return 1;
}

int multiply_many_matrix_simd(const std::vector<Matrix>& lhs, const std::vector<Matrix>& rhs, std::vector<Matrix>& result)
{
	assert(lhs.size() == rhs.size());
	size_t count = lhs.size();
	result.resize(count);

	for (size_t i = 0; i < count; i++)
	{
		multiply_matrix_simd(lhs[i], rhs[i], result[i], (lhs[i].row == 4 && rhs[i].col == 4));
	}

	return 2;
}

int multiply_many_matrix_scalar(const std::vector<Matrix>& lhs, const std::vector<Matrix>& rhs, std::vector<Matrix>& result)
{
	assert(lhs.size() == rhs.size());
	size_t count = lhs.size();
	result.resize(count);
	for (size_t i = 0; i < count; i++)
	{
		multiply_matrix_scalar(lhs[i], rhs[i], result[i], (lhs[i].row == 4 && rhs[i].col == 4));
	}

	return 2;
}

