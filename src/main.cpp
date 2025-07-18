//
//  main.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/15/25.
//
#include "Predefine.h"
#include "common.h"
#include "simd.h"
#include "particle.h"
#include "utility.h"

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    int testIndex = 2; // command args 사용하기 귀찮으면 이 값을 원하는 케이스 인덱스로 변경하세요.
    if (argc > 1)
    {
        testIndex = std::stoi(argv[1]);
    }

    Random::Init();

    std::vector<const char*> name;
    std::vector<size_t> runtime;
    std::vector<time_t> result;

    size_t testCount = 0;
    volatile int returnIndex;

    switch (testIndex)
    {
        case 0:
        case 1:
        case 2:
            testCount = 2;
            break;
        case 3:
            testCount = 3;
            break;
        default:
            std::cout << "지원하지 않는 테스트 인덱스입니다." << std::endl;
            return -1; // 프로세스 중료
    }

    assert(testCount > 0);
    name.resize(testCount);
    runtime.resize(testCount);
    result.resize(testCount);

    switch (testIndex)
    {
        case 0:
        {
            name[0] = FUNC_NAME(sum_row_major);
            name[1] = FUNC_NAME(sum_col_major);

            const int N = 10000;
            const int M = 10000;

            int** arr = new int*[N];
            for (int i = 0; i < N; i++)
            {
                arr[i] = new int[M];
                for (int j = 0; j < M; j++)
                {
                    arr[i][j] = Random::NextInt32(-512, 512);
                }
            }
            WARM_UP_CACHE_START();
            sum_row_major(arr, N, M);
            sum_col_major(arr, N, M);
            WARM_UP_CACHE_END();

            ELAPSE_START(0)
            sum_row_major(arr, N, M);
            ELAPSE_END(0, result[0]);

            ELAPSE_START(1)
            sum_col_major(arr, N, M);
            ELAPSE_END(1, result[1]);
        }
        break;
        case 1:
        {
            name[0] = FUNC_NAME(multiply_matrix_simd);
            name[1] = FUNC_NAME(multiply_matrix_scalar);

            Matrix mat1(1024, 1024, false);
            Matrix mat2(1024, 1024, false);
            Matrix mat3(1024, 1024, true);

            WARM_UP_CACHE_START();
            returnIndex = multiply_matrix_simd(mat1, mat2, mat3);
            returnIndex = multiply_matrix_scalar(mat1, mat2, mat3);
            WARM_UP_CACHE_END();

            ELAPSE_START(0)
            returnIndex = multiply_matrix_simd(mat1, mat2, mat3);
            ELAPSE_END(0, result[0]);

            ELAPSE_START(1)
            returnIndex = multiply_matrix_scalar(mat1, mat2, mat3);
            ELAPSE_END(1, result[1]);
        }
        break;
        case 2:
        {
            name[0] = FUNC_NAME(multiply_many_matrix_simd);
            name[1] = FUNC_NAME(multiply_many_matrix_scalar);

            size_t matrixCount = 10'000'000;

            std::vector<Matrix> mat1(matrixCount);
            std::vector<Matrix> mat2(matrixCount);
            std::vector<Matrix> mat3(matrixCount);

            WARM_UP_CACHE_START();
            returnIndex = multiply_many_matrix_simd(mat1, mat2, mat3);
            returnIndex = multiply_many_matrix_scalar(mat1, mat2, mat3);
            WARM_UP_CACHE_END();

            ELAPSE_START(0)
            returnIndex = multiply_many_matrix_simd(mat1, mat2, mat3);
            ELAPSE_END(0, result[0]);

            ELAPSE_START(1)
            returnIndex = multiply_many_matrix_scalar(mat1, mat2, mat3);
            ELAPSE_END(1, result[1]);
        }
        break;
        case 3:
        {
            name[0] = FUNC_NAME(update_particle_position_aos_linked_list);
            name[1] = FUNC_NAME(update_particle_position_aos_array);
            name[2] = FUNC_NAME(update_particle_position_soa_array);

            const size_t particleCount = 2'000'000;
            const size_t frameCount    = 100;
            std::list<Particle> particle_aos_linked_list(particleCount);
            Particle* particle_aos_array = new Particle[particleCount];
            ParticleSOA particle_soa_array(particleCount);

            float deltaTime = Random::NextFloat(0.001f, 0.015f);

            WARM_UP_CACHE_START();
            returnIndex = update_particle_position(particle_aos_linked_list, particleCount, deltaTime);
            returnIndex = update_particle_position(particle_aos_array, particleCount, deltaTime);
            returnIndex = update_particle_position(particle_soa_array, particleCount, deltaTime);
            WARM_UP_CACHE_END();

            ELAPSE_START(0);
            for (int i = 0; i < frameCount; i++)
                returnIndex = update_particle_position(particle_aos_linked_list, particleCount, deltaTime);
            ELAPSE_END(0, result[0]);

            ELAPSE_START(1);
            for (int i = 0; i < frameCount; i++)
                returnIndex = update_particle_position(particle_aos_array, particleCount, deltaTime);
            ELAPSE_END(1, result[1]);

            ELAPSE_START(2)
            for (int i = 0; i < frameCount; i++)
                returnIndex = update_particle_position(particle_soa_array, particleCount, deltaTime);
            ELAPSE_END(2, result[2]);

            VALIDATE(returnIndex, testIndex);
        }
        break;
        case 4:
        {
        }
        break;
            //...
        default:
            break;
    }

    std::cout << "---------------------------------" << std::endl;
    for (int i = 0; i < testCount; i++)
    {
        std::cout << "## " << i << "번 함수 " << name[i] << " 수행 시간 : " << result[i] / 1000 << " 밀리초" << std::endl;
        std::cout << "---------------------------------" << std::endl;
    }

    std::sort(result.begin(), result.end());

    std::cout << "가장 최적화된 방식은 다른 방식(들)보다 ";
    for (int i = 1; i < result.size(); i++)
    {
        std::cout << (i > 1 ? ", " : "");
        std::cout << (float)(result[i] - result[0]) / (float)result[i] * 100.0f << "%";
    }
    std::cout << " 더 효율적입니다" << std::endl;

    return 0;
}
