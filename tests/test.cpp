#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <unistd.h>
#include "../algoritmos.h"

#define L 101

using namespace chrono;

int main() {
    // tamaño de la tabla de hashing
    int l = 23;
    int hashTableSize = (1 << l);

    // número primo para las funciones de hashing
    int k = 30;
    int mersennePrime = (1 << k) - 1;

    unsigned seed = static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) + static_cast<unsigned>(getpid());
    mt19937 randomGenerator(seed);

    ofstream archivo("resultados.txt"); // Abre un archivo en modo escritura

    for (int n = 5000000; n <= 50000000; n += 5000000) {
        archivo <<"N: "<< n << endl;
        
        vector<Point> randomPoints = generateRandomPoints(n, randomGenerator);
        vector<Point> randomPointsDAC(n);

        copy(randomPoints.begin(), randomPoints.end(), randomPointsDAC.begin());

        vector<double> timesDAC(L);
        vector<double> timesUniversalHash(L);
        vector<double> timesMersenneHash(L);
        vector<double> timesFasterFunctionsHash(L);
        vector<double> timesGenericHash(L);

        // Definir el rango de valores para a y b
        uniform_int_distribution<int> aDist(1, mersennePrime - 1);
        uniform_int_distribution<int> bDist(0, mersennePrime - 1);

        int a = aDist(randomGenerator);
        int b = bDist(randomGenerator);

        for (int i = 0; i < L; i++) {
            archivo << "Consulta número " << i+1 << endl;

            cout << "-------------------------------- Divide and Conquer --------------------------------" << endl;  
            archivo << "-------------------------------- Divide and Conquer --------------------------------" << endl;  
            
            auto beginDAC = high_resolution_clock::now();
            double resDAC = findMinDistanceDivideAndConquer(randomPointsDAC);
            auto endDAC = high_resolution_clock::now();

            if (i == 0) {
                cout << resDAC << endl;
                archivo << "Resultado: " << resDAC << endl;
            }

            double timeDAC = duration_cast<milliseconds>(endDAC - beginDAC).count();
            timesDAC[i] = timeDAC;

            cout << "tiempo: " << timeDAC << endl;

            cout << "-------------------------------- Universal Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Universal Hashing --------------------------------" << endl;  
            
            auto beginUniv = high_resolution_clock::now();
            double resUniv = findMinDistanceUniversal(randomPoints, a, b, mersennePrime, hashTableSize);
            auto endUniv = high_resolution_clock::now();

            if (i == 0) {
                cout << resUniv << endl;
                archivo << "Resultado: " << resUniv << endl;
            }

            double timeUniv = duration_cast<milliseconds>(endUniv - beginUniv).count();
            timesUniversalHash[i] = timeUniv;

            cout << "tiempo: " << timeUniv << endl;

            cout << "-------------------------------- Mersenne Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Mersenne Hashing --------------------------------" << endl;  
            
            auto beginMer = high_resolution_clock::now();
            double resMer = findMinDistanceMersenne(randomPoints, mersennePrime, hashTableSize);
            auto endMer = high_resolution_clock::now();

            if (i == 0) {
                cout << resMer << endl;
                archivo << "Resultado: " << resMer << endl;
            }

            double timeMer = duration_cast<milliseconds>(endMer - beginMer).count();
            timesMersenneHash[i] = timeMer;

            cout << "tiempo: " << timeMer << endl;

            cout << "-------------------------------- Faster functions Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Faster functions Hashing --------------------------------" << endl;  
            
            auto beginFF = high_resolution_clock::now();
            double resFF = findMinDistanceFasterFunctions(randomPoints, mersennePrime, k, hashTableSize);
            auto endFF = high_resolution_clock::now();

            if (i == 0) {
                cout << resFF << endl;
                archivo << "Resultado: " << resFF << endl;
            }

            double timeFF = duration_cast<milliseconds>(endFF - beginFF).count();
            timesFasterFunctionsHash[i] = timeFF;

            cout << "tiempo: " << timeFF << endl;

            cout << "-------------------------------- Generic Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Generic Hashing --------------------------------" << endl;  

            auto beginGen = high_resolution_clock::now();
            double resGen = findMinDistanceGeneric(randomPoints, hashTableSize);
            auto endGen = high_resolution_clock::now();

            if (i == 0) {
                cout << resGen << endl;
                archivo << "Resultado: " << resGen << endl;
            }

            double timeGen = duration_cast<milliseconds>(endGen - beginGen).count();
            timesGenericHash[i] = timeGen;

            cout << "tiempo: " << timeGen << endl;
            
            cout << endl;
        }

        // Calcula el tiempo promedio de ejecución de hashing Universal
        double averageTimeDAC = accumulate(timesDAC.begin(), timesDAC.end(), 0.0) / L;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing Universal: " << averageTimeDAC << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing Universal: " << averageTimeDAC << endl;

        // Calcula el tiempo promedio de ejecución de hashing Universal
        double averageTimeUniv = accumulate(timesUniversalHash.begin(), timesUniversalHash.end(), 0.0) / L;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing Universal: " << averageTimeUniv << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing Universal: " << averageTimeUniv << endl;
        
        // Calcula el tiempo promedio de ejecución de hashing de primos de Mersenne
        double averageTimeMer = accumulate(timesMersenneHash.begin(), timesMersenneHash.end(), 0.0) / L;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing de primos de Mersenne: " << averageTimeMer << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing de primos de Mersenne: " << averageTimeMer << endl;
        
        // Calcula el tiempo promedio de ejecución de funciones más rápidas
        double averageTimeFF = accumulate(timesFasterFunctionsHash.begin(), timesFasterFunctionsHash.end(), 0.0) / L;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing de funciones más rápidas: " << averageTimeFF << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing de funciones más rápidas: " << averageTimeFF << endl;
        
        // Calcula el tiempo promedio de ejecución de un hashing genérico
        double averageTimeGen = accumulate(timesGenericHash.begin(), timesGenericHash.end(), 0.0) / L;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing Genérico: " << averageTimeGen << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing Genérico: " << averageTimeGen << endl;

        archivo.close();

        if (hashTableSize < mersennePrime) hashTableSize <<= 1;
        randomGenerator.seed(static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) + static_cast<unsigned>(getpid()));
    }
    
    return 0;
}
