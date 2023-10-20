#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <cmath>

#define getRandom(max) rand() % max

const int MAX_SIZE = 50;

short int** makeMatrix() {
    std::srand(std::time(nullptr));

    short int** matrix = new short int* [MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i)
        matrix[i] = new short int[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; ++i)
        for (int j = 0; j < MAX_SIZE; ++j)
            matrix[i][j] = getRandom(2) ? 1 : -1;

    return matrix;
}

int sumNeighbours(short int** matrix, int indexI, int indexJ) {
    const int J = 1;
    int sum = matrix[(indexI + 1) % MAX_SIZE][indexJ] +
        matrix[(indexI - 1 + MAX_SIZE) % MAX_SIZE][indexJ] +
        matrix[indexI][(indexJ + 1) % MAX_SIZE] +
        matrix[indexI][(indexJ - 1 + MAX_SIZE) % MAX_SIZE];

    return -J * sum;
}

void MonteKarlo(short int** matrix) {
    std::ofstream foutEnergy("dataEnergy.txt");
    std::ofstream foutMagnetism("dataMagnetism.txt");
    std::srand(std::time(nullptr));

    int N = (MAX_SIZE * MAX_SIZE);
    double T = 0.01;
    while (T < 4) {
        int countIter = 0;
        while (countIter < 1e6) {
            int indexI = getRandom(MAX_SIZE);
            int indexJ = getRandom(MAX_SIZE);

            short int mainSpin = matrix[indexI][indexJ];
            int oldEnergy = sumNeighbours(matrix, indexI, indexJ) * mainSpin; //E1
            int newEnergy = -oldEnergy; //E2

            if (oldEnergy > newEnergy) {
                matrix[indexI][indexJ] *= -1;
            }
            else {
                if (std::exp(-(newEnergy - oldEnergy) / T) > ((double)std::rand() / RAND_MAX))
                    matrix[indexI][indexJ] *= -1;
            }

            ++countIter;
        }

        double sumEnergy = 0;
        double magnetism = 0;
        for (int i = 0; i < MAX_SIZE; ++i)
            for (int j = 0; j < MAX_SIZE; ++j) {
                sumEnergy += matrix[i][j] * sumNeighbours(matrix, i, j);
                magnetism += matrix[i][j];
            }

        sumEnergy /= 2;
        magnetism /= N;
        foutEnergy << sumEnergy << "\n";
        foutMagnetism << magnetism << "\n";
        T += 0.1;
    }
    foutEnergy.close();
}

int main() {
    unsigned int start_time = std::clock();
    short int** matrix = makeMatrix();
    MonteKarlo(matrix);
    unsigned int start_end = std::clock();
    std::cout << (start_end - start_time) / CLOCKS_PER_SEC;
    return 0;
}

#undef getRandom(max)
