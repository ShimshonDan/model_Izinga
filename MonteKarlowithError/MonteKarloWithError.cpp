#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <cmath>

#define getRandom(max) rand() % max

const int MAX_SIZE = 4;

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

const int MAX_SIZE_SQUARE = MAX_SIZE * MAX_SIZE;
std::ofstream fout("dataAnswer.txt");
void mainAlgorithm(short int** matrix, int* matrixEnergy,
    double& T, int maxIterT, int maxIterMK,
    double& sum
    )
{
    int iterT = 0;
    double sumEnergy = 0;
    int indexEnergy = 0;
    while (iterT < maxIterT) {
        ++iterT;
        int iterMK = 0;
        while (iterMK < maxIterMK) {
            sumEnergy = 0;

            int indexI = getRandom(MAX_SIZE);
            int indexJ = getRandom(MAX_SIZE);
            
            short int mainSpin = matrix[indexI][indexJ];
            int oldEnergy = sumNeighbours(matrix, indexI, indexJ);
            int newEnergy = -oldEnergy;

            if (oldEnergy > newEnergy) {
                matrix[indexI][indexJ] *= -1;
            }
            else {
                if (std::exp(-(newEnergy - oldEnergy) / T) > ((double)std::rand() / RAND_MAX))
                    matrix[indexI][indexJ] *= -1;
            }

            for (int i = 0; i < MAX_SIZE; ++i)
                for (int j = 0; j < MAX_SIZE; ++j)
                    sumEnergy += matrix[i][j] * sumNeighbours(matrix, i, j);
            sumEnergy /= 2;

            matrixEnergy[indexEnergy] = sumEnergy;
            sum += sumEnergy;

            ++iterMK;
            ++indexEnergy;
        }

        double sumEnergy = 0;
        double sumEnergySquare = 0;
        double magnetism = 0;

        for (int i = 0; i < MAX_SIZE; ++i)
            for (int j = 0; j < MAX_SIZE; ++j) {
                sumEnergy += matrix[i][j] * sumNeighbours(matrix, i, j);
                sumEnergySquare += sumEnergy * sumEnergy;
                magnetism += matrix[i][j];
            }
        sumEnergy /= 2;
        sumEnergySquare /= 4;
        magnetism /= MAX_SIZE_SQUARE;

        fout << std::fixed << std::setprecision(2) << std::setw(6) << T << "\t" << sumEnergy << "\t"
            << magnetism << "\t" << ((sumEnergySquare)-(sumEnergy * sumEnergy)) / (T * T) << "\n";

        T += 0.1;
    }
}

const int MAX_ITER_TEMP = 40;
const int MAX_ITER_MK = 10E6;
const int MAX_SIZE_ARRAY = MAX_ITER_MK * MAX_ITER_TEMP / 2;
void MonteKarlo(short int** matrix) {
    fout << "\n\tT\t E(T)\t M(T)\t C(T)\n";

    double T = 0.01;
    int* fullMatrixEnergyFirtsPart = new int [MAX_SIZE_ARRAY];
    int* fullMatrixEnergySecondPart = new int [MAX_SIZE_ARRAY];
    
    double avarangeEnergy = 0;
    mainAlgorithm(matrix, fullMatrixEnergyFirtsPart, T, MAX_ITER_TEMP / 2, MAX_ITER_MK, avarangeEnergy);
    mainAlgorithm(matrix, fullMatrixEnergySecondPart, T, MAX_ITER_TEMP / 2, MAX_ITER_MK, avarangeEnergy);
    
    avarangeEnergy /= MAX_ITER_MK;

    int index = 0;
    double summEnergy = 0;
    while (index < MAX_SIZE_ARRAY) {
        double brackets = fullMatrixEnergyFirtsPart[index] - avarangeEnergy;
        summEnergy += brackets * brackets;
        brackets = fullMatrixEnergySecondPart[index] - avarangeEnergy;
        summEnergy += brackets * brackets;
        ++index;
    }

    fout << "\nError system: " << std::fixed << std::setprecision(10) << std::sqrt((1.0 / (MAX_ITER_MK - 1)) * summEnergy) << "\n\n";
}

int main() {
    unsigned int start_time = std::clock();
    short int** matrix = makeMatrix();
    fout << "\tMain matrix\n";
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j)
            fout << std::setw(5) << matrix[i][j] << " ";
        fout << "\n";
    }

    MonteKarlo(matrix);
    unsigned int start_end = std::clock();
    fout << "Time programm: " << (start_end - start_time) / CLOCKS_PER_SEC;

    for (int i = 0; i < MAX_SIZE; ++i)
        delete[] matrix[i];
    delete[] matrix;

    fout.close();
    return 0;
}

#undef getRandom(max)
