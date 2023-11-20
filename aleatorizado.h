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

double calculateDistance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<pair<Point, Point>> generateRandomPointPairs(int n) {
    vector<pair<Point, Point>> pointPairs;
    srand(time(nullptr));

    for (int i = 0; i < n; ++i) {
        double x1 = static_cast<double>(rand()) / RAND_MAX;
        double y1 = static_cast<double>(rand()) / RAND_MAX;
        double x2 = static_cast<double>(rand()) / RAND_MAX;
        double y2 = static_cast<double>(rand()) / RAND_MAX;

        Point p1(x1, y1);
        Point p2(x2, y2);

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
    vector<pair<Point, Point>> pointPairs = generateRandomPointPairs(n);

    double minDistance = numeric_limits<double>::max();

    for (const auto& pair : pointPairs) {
        double distance = calculateDistance(pair.first, pair.second);
        if (distance < minDistance) {
            minDistance = distance;
        }
    }

    double d = minDistance;

    // Utilizar la Hash Table con el método de hashing utilizando el operador módulo
    HashTableModulo hashTable(d);

    for (const auto& pair : pointPairs) {
        hashTable.insert(pair.first);
        hashTable.insert(pair.second);
    }

    // Comparar distancias entre puntos en la misma casilla y casillas vecinas

    return minDistance;
}


