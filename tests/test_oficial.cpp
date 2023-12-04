#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <unistd.h>
#include "../algoritmos.h"

#define C 5

using namespace chrono;

int main() {
    // tamaño de la tabla de hashing
    int l = 7;
    int hashTableSize = (1 << (K-l));

    unsigned seed = static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) + static_cast<unsigned>(getpid());
    mt19937 randomGenerator(seed);

    ofstream archivo("resultados.txt"); // Abre un archivo en modo escritura

    for (int n = 50000; n <= 50000000; n += 50000) {
        archivo <<"N: "<< n << endl;
        
        vector<Point> points = generateRandomPoints(n, randomGenerator);

        vector<double> timesUniversalHash(C);
        vector<double> timesMersenneHash(C);
        vector<double> timesFasterFunctionsHash(C);
        vector<double> timesDAC(C);

        for (int i = 0; i < C; i++) {
            archivo << "Consulta número " << i+1 << endl;
            cout << "Consulta número " << i+1 << endl;

            // Definir el rango de valores para a y b
            uniform_int_distribution<int> aDist(1, MP - 1);
            uniform_int_distribution<int> bDist(0, MP - 1);

            int a = aDist(randomGenerator);
            int b = bDist(randomGenerator);

            cout << "-------------------------------- Universal Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Universal Hashing --------------------------------" << endl;  
            
            auto beginUniv = high_resolution_clock::now();
            double resUniv = findMinDistanceHash(points, universalHash, a, b, hashTableSize);
            auto endUniv = high_resolution_clock::now();

            cout << resUniv << endl;
            archivo << "Resultado: " << resUniv << endl;
            
            double timeUniv = duration_cast<milliseconds>(endUniv - beginUniv).count();
            timesUniversalHash[i] = timeUniv;

            cout << "Tiempo: " << timeUniv << endl;
            
            cout << "-------------------------------- Mersenne Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Mersenne Hashing --------------------------------" << endl;  
            
            auto beginMer = high_resolution_clock::now();
            double resMer = findMinDistanceHash(points, mersennePrimeHash, a, b, hashTableSize);
            auto endMer = high_resolution_clock::now();

            cout << resMer << endl;
            archivo << "Resultado: " << resMer << endl;
            
            double timeMer = duration_cast<milliseconds>(endMer - beginMer).count();
            timesMersenneHash[i] = timeMer;

            cout << "Tiempo: " << timeMer << endl;

            cout << "-------------------------------- Faster functions Hashing --------------------------------" << endl;  
            archivo << "-------------------------------- Faster functions Hashing --------------------------------" << endl;  
            
            auto beginFF = high_resolution_clock::now();
            double resFF = findMinDistanceHash(points, fasterFunctionsHash, a, b, hashTableSize);
            auto endFF = high_resolution_clock::now();

            cout << resFF << endl;
            archivo << "Resultado: " << resFF << endl;

            double timeFF = duration_cast<milliseconds>(endFF - beginFF).count();
            timesFasterFunctionsHash[i] = timeFF;

            cout << "Tiempo: " << timeFF << endl;

            cout << "-------------------------------- Divide and Conquer --------------------------------" << endl;  
            archivo << "-------------------------------- Divide and Conquer --------------------------------" << endl;  
            
            auto beginDAC = high_resolution_clock::now();
            double resDAC = findMinDistanceDivideAndConquer(points);
            auto endDAC = high_resolution_clock::now();

            cout << resDAC << endl;
            archivo << "Resultado: " << resDAC << endl;

            double timeDAC = duration_cast<milliseconds>(endDAC - beginDAC).count();
            timesDAC[i] = timeDAC;

            cout << "Tiempo: " << timeDAC << endl;


            cout << endl;
            archivo << endl;
        }

        // Calcula el tiempo promedio de ejecución de hashing Universal
        double averageTimeUniv = accumulate(timesUniversalHash.begin(), timesUniversalHash.end(), 0.0) / C;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing Universal: " << averageTimeUniv << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing Universal: " << averageTimeUniv << endl;
        
        // Calcula el tiempo promedio de ejecución de hashing de primos de Mersenne
        double averageTimeMer = accumulate(timesMersenneHash.begin(), timesMersenneHash.end(), 0.0) / C;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing de primos de Mersenne: " << averageTimeMer << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing de primos de Mersenne: " << averageTimeMer << endl;
        
        // Calcula el tiempo promedio de ejecución de funciones más rápidas
        double averageTimeFF = accumulate(timesFasterFunctionsHash.begin(), timesFasterFunctionsHash.end(), 0.0) / C;
        cout << "n: " << n << ", promedio Aleatorizado con Hashing de funciones más rápidas: " << averageTimeFF << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Hashing de funciones más rápidas: " << averageTimeFF << endl;

        // Calcula el tiempo promedio de ejecución de dividir para reinar
        double averageTimeDAC = accumulate(timesDAC.begin(), timesDAC.end(), 0.0) / C;
        cout << "n: " << n << ", promedio Aleatorizado con Dividir para reinar: " << averageTimeDAC << endl;
        archivo << "n: " << n << ", promedio Aleatorizado con Dividir para reinar: " << averageTimeDAC << endl;

        if (hashTableSize < MP) {
            hashTableSize <<= 1;
            l--;
        }
        randomGenerator.seed(static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) + static_cast<unsigned>(getpid()));
    }
    
    archivo.close();

    return 0;
}
