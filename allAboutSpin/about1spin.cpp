#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <random>
#include <ctime>

#include <cmath>

#define getRandom(max) rand()%max
const int MAX_SIZE = 4; //50

std::ofstream fout("testMatrix.txt");
void writeToFile(short int** matrix, int sizeColumn, int sizeString, std::string title) {
    fout << title << "\n";

    for (int i = 0; i < sizeColumn; ++i) {
        for (int j = 0; j < sizeString; ++j)
            fout << std::setw(5) << matrix[i][j] << " ";
        fout << std::endl;
    }
    fout << "________________________" << "\n";
}

short int** makeMatrix() {
    std::srand(std::time(nullptr));

    short int** matrix = new short int* [MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i)
        matrix[i] = new short int[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; ++i)
        for (int j = 0; j < MAX_SIZE; ++j)
            matrix[i][j] = getRandom(2) ? 1 : -1;

    return  matrix;
}


bool checkIndexMore(int index) {
    //checks that the element is at the extreme bottom/right position
    return index == MAX_SIZE;
}

bool checkIndexLess(int index) {
    //checks that the element is at the top/rightmost position
    return index < 0;
}

void addElement(short int** matrix, int iter,
    int indexI, int indexJ,
    short int data)
{
    //writes data to an array
    matrix[iter][0] = indexI;
    matrix[iter][1] = indexJ;
    matrix[iter][2] = data;
}

short int** findNeighbours(short int indexI, short int indexJ, short int** matrix) {
    //collects data about neighbors
    short int** neighbours = new short int* [5];
    for (int i = 0; i < 5; ++i)
        neighbours[i] = new short int[3];

    //add first element in matrix - neighbours main spin
    neighbours[0][0] = indexI;
    neighbours[0][1] = indexJ;
    neighbours[0][2] = matrix[indexI][indexJ];

    int nIndex = indexI - 1;
    if (checkIndexLess(nIndex))
        nIndex = MAX_SIZE - 1;
    addElement(neighbours, 1, nIndex, indexJ, matrix[nIndex][indexJ]); // add upper

    nIndex = indexI + 1;
    if (checkIndexMore(nIndex))
        nIndex = 0;
    addElement(neighbours, 2, nIndex, indexJ, matrix[nIndex][indexJ]); //add lower

    nIndex = indexJ - 1;
    if (checkIndexLess(nIndex))
        nIndex = MAX_SIZE - 1;
    addElement(neighbours, 3, indexI, nIndex, matrix[indexI][nIndex]); //add left

    nIndex = indexI + 1;
    if (checkIndexMore(nIndex))
        nIndex = 0;
    addElement(neighbours, 4, indexI, nIndex, matrix[indexI][nIndex]); //add right

    return neighbours;
}

const int J = 1;
int countEnergyRandomSpin(short int** matrix) {
    int sum = 0;
    for (int i = 1; i < 5; ++i)
        sum += matrix[0][2] * matrix[i][2];

    return -J * sum;
}

double countEnergyFull(short int** matrix) {
    int sum = 0;
    for (int i = 0; i < MAX_SIZE; ++i)
        for (int j = 0; j < MAX_SIZE; ++j)
            sum += countEnergyRandomSpin(findNeighbours(i, j, matrix));

    return sum / 2;
}

double countEnergyAverage(double fullEnergy) {
    return (2 * fullEnergy) / (MAX_SIZE * MAX_SIZE);
}

double countMagnetism(short int** matrix);

void allFunctionFor4();

int main() {
    allFunctionFor4();

    return 0;
}

void allFunctionFor4() {
    short int** mainMatrix = makeMatrix();
    writeToFile(mainMatrix, MAX_SIZE, MAX_SIZE,
        "-------Main Matrix------");

    short int indexI = getRandom(MAX_SIZE);
    short int indexJ = getRandom(MAX_SIZE);

    //findNeighbours(0, 0, matrix);
    //findNeighbours(3, 3, matrix);
    short int** neighbours = findNeighbours(indexI, indexJ, mainMatrix);
    writeToFile(neighbours, 5, 3,
        "-------Neighbours------");

    fout << "---------Energy----------\n";
    fout << "Energy random spins: " << countEnergyRandomSpin(neighbours) << "\n";
    fout << "Full energy system: " << countEnergyFull(mainMatrix) << "\n";
    fout << "Average energy: " << countEnergyAverage(countEnergyFull(mainMatrix));
    fout << "\n_________________________" << "\n";

    fout << "--------Magnetism--------\n";
    fout << "Magnetism: " << countMagnetism(mainMatrix);
    fout << "\n_________________________" << "\n";

    for (int i = 0; i < 5; ++i)
        delete neighbours[i];
    delete[] neighbours;

    for (int i = 0; i < MAX_SIZE; ++i)
        delete mainMatrix[i];
    delete[] mainMatrix;

    fout.close();
}

double countMagnetism(short int** matrix) {
    double sum = 0;
    for (int i = 0; i < MAX_SIZE; ++i)
        for (int j = 0; j < MAX_SIZE; ++j)
            sum += matrix[i][j];

    return sum / MAX_SIZE;
}
#undef getRandom