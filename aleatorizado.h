#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point {
    double x, y;
    Point(double _x, double _y) : x(_x), y(_y) {}
};

double distance(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<pair<Point, Point>> generateRandomPointPairs(int n) {
    vector<pair<Point, Point>> pointPairs;
    srand(time(nullptr));

    for (int i = 0; i < n; ++i) {
        double x1 = static_cast<double>(rand()) / RAND_MAX * 100;
        double y1 = static_cast<double>(rand()) / RAND_MAX * 100;
        double x2 = static_cast<double>(rand()) / RAND_MAX * 100;
        double y2 = static_cast<double>(rand()) / RAND_MAX * 100;

        Point p1(x1, y1);
        Point p2(x2, y2);

        // Asegurarse de que los puntos sean distintos
        while (distance(p1, p2) == 0) {
            x2 = static_cast<double>(rand());
            y2 = static_cast<double>(rand());
            p2 = Point(x2, y2);
        }

        pointPairs.push_back(make_pair(p1, p2));
    }

    return pointPairs;
}





// Implementación de la Hash Table utilizando el operador módulo
class HashTableModulo {
public:
    HashTableModulo(double cellSize) : cellSize_(cellSize) {}

    void insert(const Point& point) {
        int key = hashFunction(point);
        hashTable_[key].push_back(point);
    }

    vector<Point>& getPoints(const Point& point) {
        int key = hashFunction(point);
        return hashTable_[key];
    }

private:
    int hashFunction(const Point& point) {
        int x = static_cast<int>(point.x / cellSize_);
        int y = static_cast<int>(point.y / cellSize_);
        return x * 31 + y;  // Ajusta el valor 31 según sea necesario
    }

    double cellSize_;
    unordered_map<int, vector<Point>> hashTable_;
};


double randomizedAlgorithm(int n) {
    // se generan los n pares
    vector<pair<Point, Point>> pointPairs = generateRandomPointPairs(n);

    // Distancia mínima entre los puntos de cada par
    double d = numeric_limits<double>::max();

    for (const auto& pair : pointPairs) {
        double dist = distance(pair.first, pair.second);
        if (dist < d) {
            d = dist;
        }
    }

    /////////////////////////
    // HashTableModulo hashTable(d): Crea una instancia de la clase HashTableModulo 
    // utilizando el valor de d, que es la distancia mínima entre los puntos en los pares

    // Utilizar la Hash Table con el método de hashing utilizando el operador módulo
    HashTableModulo hashTable(d);


    // Inserción de puntos en la Hash Table: Itera sobre todos los pares de puntos y 
    // los inserta en la Hash Table. Esto agrupa los puntos en celdas de la grilla basada 
    // en d utilizando el método de hashing con el operador módulo.
    for (const auto& pair : pointPairs) {
        hashTable.insert(pair.first);
        hashTable.insert(pair.second);
    }


    /*
    Comparación de distancias entre puntos en la misma casilla y casillas vecinas:
    Falta la implementación específica de esta parte. Aquí deberías completar la lógica 
    para comparar las distancias entre puntos en la misma casilla y casillas vecinas.
    */
    
    // Comparar distancias entre puntos en la misma casilla y casillas vecinas
    for (const auto& pair : pointPairs) {
        // Obtener los puntos en la misma celda que el primer punto del par
        vector<Point>& sameCellPoints = hashTable.getPoints(pair.first);
    
        // Comparar distancias entre los puntos en la misma celda
        for (const Point& point : sameCellPoints) {
            double dist = distance(pair.first, point);
            if (dist < d) {
                d = dist;
            }
        }
    
        // Comparar distancias entre los puntos en las casillas vecinas
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) {
                    // No necesitamos comparar con la casilla actual
                    continue;
                }
    
                // Obtener los puntos en la celda vecina
                vector<Point>& neighborCellPoints = hashTable.getPoints(Point(pair.first.x + i, pair.first.y + j));
    
                // Comparar distancias entre los puntos en la celda vecina
                for (const Point& point : neighborCellPoints) {
                    double dist = distance(pair.first, point);
                    if (dist < d) {
                        d = dist;
                    }
                }
            }
        }
    }

    return d;
}
