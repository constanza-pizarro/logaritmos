#include "C:\Users\Vicente\Desktop\FCFM\Algoritmos\T3\logaritmos\aleatory_working.cpp"

#include <chrono>
#include <random>

#include <iostream>
#include <fstream>

using namespace std;
using namespace std::chrono;

int main() {
    
    unsigned seed = static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    mt19937 gen(seed);

    cout << "Semilla utilizada: " << seed << endl;

    std::ofstream archivo("file.txt"); // Abre el archivo en modo escritura
    vector<Point> randomPointsCpy;
    

    for (int n =5000000; n<=50000000; n= n+5000000){

       
        cout <<"N ="<< n << endl;
        archivo <<"N ="<< n <<std::endl;
        double duration_total = 0;
        double duration_total2 = 0;
        double duration_total3 = 0;



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
//
          

            archivo <<"Hashing method = Mersenne Hashing"<<std::endl;
            auto start2 = high_resolution_clock::now();
            double result2= findMinDistanceOptimizedMersenne(randomPoints);
            cout <<"distancia = "<< result2<< endl;
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<milliseconds>(stop2 - start2);
            //cout <<"Intento numero "<< intentos << " terminado" << endl;
            archivo <<"duration "<<duration2.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
            duration_total2 += duration2.count();


            archivo <<"Hashing method = Native"<<std::endl;
            auto start3 = high_resolution_clock::now();
            double result3= findMinDistanceOptimized(randomPointsCpy);
            cout <<"distancia = "<< result3<< endl;
            auto stop3 = high_resolution_clock::now();
            auto duration3 = duration_cast<milliseconds>(stop3 - start3);
            //cout <<"Intento numero "<< intentos << " terminado" << endl;
            archivo <<"duration "<<duration3.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
            duration_total3 += duration3.count();


            cout <<"Intento numero "<< intentos << " terminado" << endl;
            archivo <<"Intento numero "<< intentos << " terminado" <<std::endl; // Escribe cada número seguido de un salto de línea
            archivo <<"\n"<<std::endl;

            //duration_total += duration.count();
            if (intentos == 3){ 
                                archivo <<"duration mean Mersenne "<<duration_total2/3 <<" ms" <<std::endl;
                                archivo <<"duration mean Native "<<duration_total3/3 <<" ms" <<std::endl;
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
