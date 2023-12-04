//#include "C:\Users\Vicente\Desktop\FCFM\Algoritmos\T3\logaritmos\aleatory_working.cpp"

#include "../aleatory_working.h"
#include <chrono>
#include <random>

#include <iostream>
#include <fstream>
#include <unistd.h>  // Para getpid()

using namespace std;
using namespace std::chrono;

int main() {
    
    unsigned seed = static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) +
                static_cast<unsigned>(getpid());  // Agrega el ID de proceso
    mt19937 gen(seed);


    cout << "Semilla utilizada: " << seed << endl;

    ofstream archivo("file.txt"); // Abre el archivo en modo escritura
    vector<Point> randomPointsCpy;
    

    for (int n =50000; n<=50000000; n= n+5000){

        cout <<"N ="<< n << endl;
        archivo <<"N ="<< n <<std::endl;
        double duration_total = 0;
        double duration_total2 = 0;
        double duration_total3 = 0;
        double duration_total4 = 0;


        for(int intentos= 1; intentos <=3; intentos++){

            vector<Point> randomPoints = generateRandomPoints(n, gen);
            randomPointsCpy=randomPoints;



            //archivo <<"Hashing method = City Hashing"<<std::endl;
            //auto start = high_resolution_clock::now();
            //double result= findMinDistanceOptimizedCustom(randomPointsCpy, generateGridCity);
            //auto stop = high_resolution_clock::now();
            //auto duration = duration_cast<milliseconds>(stop - start);
            ////cout <<"Intento numero "<< intentos << " terminado" << endl;
            //archivo <<"duration "<<duration.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
   
            // primo más grande que entra en 32 bits
            const int mersenne_prime = (1 << 31) - 1;

            cout << "-------------------------------- Hashing Mersenne --------------------------------" << endl;
            archivo << "Hashing method = Mersenne Hashing" << endl;
            auto start2 = high_resolution_clock::now();
            double result2= findMinDistanceOptimizedMersenne(randomPoints);
            cout <<"distancia = "<< result2<< endl;
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<milliseconds>(stop2 - start2);
            //cout <<"Intento numero "<< intentos << " terminado" << endl;
            archivo <<"duration "<<duration2.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
            duration_total2 += duration2.count();
            

            cout << "-------------------------------- Hashing Universal --------------------------------" << endl;
            // Definir el rango de valores para a y b
            uniform_int_distribution<int> dist_a(1, mersenne_prime - 1);
            uniform_int_distribution<int> dist_b(0, mersenne_prime - 1);

            int a = dist_a(gen);
            int b = dist_b(gen);
            int m = 1000;  // Tamaño de la tabla de hash, ajusta según sea necesario
        
            archivo << "Hashing method = Universal Hashing" << endl;
            auto start4 = high_resolution_clock::now();
            double result4= findMinDistanceOptimizedUniversal(randomPoints, a, b, mersenne_prime, m);
            cout <<"distancia = "<< result4<< endl;
            auto stop4 = high_resolution_clock::now();
            auto duration4 = duration_cast<milliseconds>(stop4 - start4);
            archivo <<"duration "<<duration4.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
            duration_total4 += duration4.count();


            cout << "-------------------------------- Hashing generico --------------------------------" << endl;
            archivo << "Hashing method = Native" << endl;
            auto start3 = high_resolution_clock::now();
            double result3= findMinDistanceOptimized(randomPointsCpy);
            cout << "distancia = " << result3 << endl;
            auto stop3 = high_resolution_clock::now();
            auto duration3 = duration_cast<milliseconds>(stop3 - start3);
            //cout <<"Intento numero "<< intentos << " terminado" << endl;
            archivo <<"duration "<<duration3.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
            duration_total3 += duration3.count();


            cout << "Intento numero " << intentos << " terminado" << endl;
            archivo << "Intento numero " << intentos << " terminado" << endl; // Escribe cada número seguido de un salto de línea
            archivo << "\n" <<endl;

            //duration_total += duration.count();
            if (intentos == 3){ 
                archivo << "duration mean Mersenne " << duration_total2/3 << " ms" << endl;
                archivo << "duration mean Native " << duration_total3/3 << " ms" << endl;
            } // Escribe cada número seguido de un salto de línea}
        }


        //auto start = high_resolution_clock::now();
        //double result= findMinDistanceOptimized(randomPoints);
        //auto stop = high_resolution_clock::now();
        //auto duration = duration_cast<milliseconds>(stop - start);
        //cout <<"Hashing generico, Distancia minima: " << result << ", Tiempo: " << duration.count() << " ms" << endl;

          // Verifica si el archivo se abrió correctamente
        // Datos de ejemplo (números double)

        // Guarda los números en el archivo
        //archivo <<"N ="<< n << " / duration "<<duration.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
        //cout << "number copied"<< endl;

       
     
    }

    archivo.close();



    //auto start2 = high_resolution_clock::now();
    //double result2= findMinDistanceOptimizedCustom(randomPoints);
    //auto stop2 = high_resolution_clock::now();
    //auto duration2 = duration_cast<milliseconds>(stop2 - start2);
    //cout <<"Hashing custom, Distancia minima: " << result2 << ", Tiempo: " << duration2.count() << " ms" << endl;


    return 0;
}
