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

    

    for (int n =5000000; n<=50000000; n= n+5000000){

       
        cout <<"N ="<< n << endl;
        archivo <<"N ="<< n <<std::endl;
        double duration_total = 0;
        for(int intentos= 0; intentos <=20; intentos++){
            vector<Point> randomPoints = generateRandomPoints(n, gen);
            auto start = high_resolution_clock::now();
            double result= findMinDistanceOptimized(randomPoints);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);
            cout <<"Intento numero "<< intentos << " terminado" << endl;
            archivo <<"duration "<<duration.count() <<"ms" <<std::endl; // Escribe cada número seguido de un salto de línea
            duration_total += duration.count();
            if (intentos == 20){ archivo <<"duration mean "<<duration_total/20 <<" ms" <<std::endl;} // Escribe cada número seguido de un salto de línea}
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
