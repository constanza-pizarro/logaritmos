#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <random>
#include <limits>
#include <cstdint>
#include <map>

// primo de Mersenne para las funciones de Hashing
#define K 30
#define MP (1 << K) - 1

using namespace std;

struct Point {
    double x, y;
};

double calculateDistance(Point p1, Point p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

vector<Point> generateRandomPoints(int n, mt19937& gen) {
    vector<Point> points;
    uniform_real_distribution<> dis(0.0, 1.0);

    while (points.size() < n) {
        double x = dis(gen);
        double y = dis(gen);

        if (x < 1.0 && y < 1.0) {
            Point p;
            p.x = x;
            p.y = y;
            points.push_back(p);
        }
    }
    return points;
}


// ------------ DIVIDIR PARA REINAR ------------

bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

double closestPairDist(vector<Point>& arr, int left, int right) {
    // Caso base donde el tamaño del conjunto es 0, 1 o 2.
    if (right - left <= 2) { 
        // si es 0 o 1, se devuelve infinito
        double min = numeric_limits<double>::infinity();
        if (right - left == 2) // si es 2, se devuelve la distancia entre ambos puntos
        min = calculateDistance(arr[left], arr[left+1]);
        return min;
    }

    // Ordenamos los puntos según su coordenada x
    sort(arr.begin() + left, arr.begin() + right, compareX);

    // Si no estamos en el caso base, entonces dividimos el conjunto 
    // en dos partes iguales usando una recta divisoria
    // Calculamos recursivamente la distancia minima en ambas partes
    int mid = (left + right) / 2;

    double Lmin = closestPairDist(arr, left, mid);
    double Rmin = closestPairDist(arr, mid, right);
    // Calculamos el minimo entre ambas soluciones
    double LRmin = min(Lmin, Rmin);

    for (int i = mid-1; i >= left; --i) {
        if (arr[mid].x - arr[i].x >= LRmin) {
            break;  // Salir si ya estamos más lejos que la distancia mínima
        }

        for (int j = mid; j < right; ++j) {
            double currDist = calculateDistance(arr[i], arr[j]);
            LRmin = min(LRmin, currDist);
        }
    }

    // retorna la distancia mínima
    return LRmin;
}

double findMinDistanceDivideAndConquer(vector<Point>& arr) {
    return closestPairDist(arr, 0, arr.size());
}


// ------------------ HASHING ------------------

int universalHash(double value, int a, int b, int m) {
    return ((static_cast<int>(a) * static_cast<int>(value) + b) % MP) % m;
}

int mersennePrimeHash(double value, int a, int b, int m) {
    return static_cast<int>(value) % MP;
}

// Funciones más rápidas
int fasterFunctionsHash(double value, int a, int b, int m) {
    int intValue = static_cast<int>(value);

    int result = (intValue & MP) + (intValue >> K);

    // Calcular result mód MP de manera eficiente
    if (result >= MP) result -= MP;

    return result;
}


// ------------------ GRILLAS ------------------

vector<Point> getNeighboringPoints(const map<pair<int, int>, vector<Point>>& gridMap, int gridX, int gridY) {
    vector<Point> neighboringPoints;
    for (int i = gridX - 1; i <= gridX + 1; ++i) {
        for (int j = gridY - 1; j <= gridY + 1; ++j) {
            pair<int, int> gridKey(i, j);
            if (gridMap.find(gridKey) != gridMap.end()) {
                const vector<Point>& points = gridMap.at(gridKey);
                neighboringPoints.insert(neighboringPoints.end(), points.begin(), points.end());
            }
        }
    }

    return neighboringPoints;
}

map<pair<int, int>, vector<Point>> groupPointsInGrid(const vector<Point>& points, double minDistance, int hashFun(double value, int a, int b, int m), int a, int b, int m) {
    map<pair<int, int>, vector<Point>> gridMap;

    for (const Point& point : points) {
        int gridX = static_cast<int>(hashFun(point.x, a, b, m) / minDistance);
        int gridY = static_cast<int>(hashFun(point.y, a, b, m) / minDistance);

        pair<int, int> gridKey(gridX, gridY);

        // Insertar el punto en la grilla correspondiente
        gridMap[gridKey].push_back(point);
    }

    return gridMap;
}

double compareDistances(const vector<Point>& points, const map<pair<int, int>, vector<Point>>& gridMap, double minDistance, int hashFun(double value, int a, int b, int m), int a, int b, int m) {
    double newMinDistance = minDistance;

    for (const Point& point : points) {
        int gridX = static_cast<int>(hashFun(point.x, a, b, m) / minDistance);
        int gridY = static_cast<int>(hashFun(point.y, a, b, m) / minDistance);

        vector<Point> neighboringPoints = getNeighboringPoints(gridMap, gridX, gridY);
        for (const Point& neighborPoint : neighboringPoints) {
            double distance = calculateDistance(point, neighborPoint);
            if (distance > 0 && distance < newMinDistance) {
                newMinDistance = distance;
            }
        }
    }

    return newMinDistance;
}

double minBetweenPairs(vector<Point> points) {
    srand(static_cast<unsigned>(time(nullptr)));
    double minDistance = numeric_limits<double>::max();

    for (int i = 0; i < points.size(); ++i) {
        int index1 = rand() % points.size();
        int index2 = rand() % points.size();
        while (index1 == index2) {
            index2 = rand() % points.size();
        }

        double distance = calculateDistance(points[index1], points[index2]);
        minDistance = min(minDistance, distance);
    }

    return minDistance;
}

double findMinDistanceHash(vector<Point> points, int hashFun(double value, int a, int b, int m), int a, int b, int m) {
    double minDistance = minBetweenPairs(points);

    map<pair<int, int>, vector<Point>> gridMap = groupPointsInGrid(points, minDistance, hashFun, a, b, m);

    return compareDistances(points, gridMap, minDistance, hashFun, a, b, m);

}
