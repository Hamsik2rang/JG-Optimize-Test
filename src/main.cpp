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

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    int testIndex = 3;
    if (argc > 1)
    {
        testIndex = std::stoi(argv[1]);
    }

    srand((unsigned int)time(NULL));

    std::vector<const char*> name;
    std::vector<size_t> runtime;
    std::vector<time_t> result;

    size_t testCount = 0;

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
                    arr[i][j] = rand() % 512;
                }
            }
            WARM_UP_CACHE_START();
            sum_row_major(arr, N, M);
            sum_col_major(arr, N, M);
            WARM_UP_CACHE_END();

            ELAPSE_START(0)
            sum_row_major(arr, N, M);
            ELAPSE_END(0, result[0]);

            // warm_Up
            sum_col_major(arr, N, M);
            ELAPSE_START(1)
            sum_col_major(arr, N, M);
            ELAPSE_END(1, result[1]);
        }
        break;
        case 1:
        {
            name[0] = FUNC_NAME(multiply_matrix_simd);
            name[1] = FUNC_NAME(multiply_matrix_scalar);
        }
        break;
        case 2:
        {
            name[0] = FUNC_NAME(multiply_many_matrix_simd);
            name[1] = FUNC_NAME(multiply_many_matrix_scalar);
        }
        break;
        case 3:
        {
            name[0] = FUNC_NAME(update_particle_position);
            name[1] = FUNC_NAME(update_particle_position);
            name[2] = FUNC_NAME(update_particle_position);

            const size_t particleCount = 15000;
            std::list<Particle> particle_aos_linked_list(particleCount);
            Particle* particle_aos_array = new Particle[particleCount];
            ParticleSOA<particleCount> particle_soa;

            float deltaTime = (float)rand();
            WARM_UP_CACHE_START();
            update_particle_position(particle_aos_linked_list, particleCount, deltaTime);
            update_particle_position(particle_aos_array, particleCount, deltaTime);
            update_particle_position(particle_soa, particleCount, deltaTime);
            WARM_UP_CACHE_END();

            ELAPSE_START(0);
            for (int i = 0; i < 1000000; i++)
                update_particle_position(particle_aos_linked_list, particleCount, deltaTime);
            ELAPSE_END(0, result[0]);

            ELAPSE_START(1);
            for (int i = 0; i < 1000000; i++)
                update_particle_position(particle_aos_array, particleCount, deltaTime);
            ELAPSE_END(1, result[1]);

            ELAPSE_START(2)
            for (int i = 0; i < 1000000; i++)
                update_particle_position(particle_soa, particleCount, deltaTime);
            ELAPSE_END(2, result[2]);
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
        std::cout << "## " << i << "번 함수 " << name[0] << " 수행 시간 : " << result[0] / 1000 << " 밀리초" << std::endl;
        std::cout << "---------------------------------" << std::endl;
    }

    return 0;
}
