#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

struct Point {
    double x, y;
};

double calculateDistance(Point p1, Point p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

vector<pair<Point, Point>> generateRandomPointPairs(int n) {
    vector<pair<Point, Point>> pairs;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0); // Rango del plano [0, 1)

    for (int i = 0; i < n; ++i) {
        Point p1, p2;
        p1.x = dis(gen);
        p1.y = dis(gen);
        p2.x = dis(gen);
        p2.y = dis(gen);
        pairs.emplace_back(make_pair(p1, p2));
    }
    return pairs;
}

double findMinDistance(const vector<pair<Point, Point>>& pairs) {
    double minDistance = numeric_limits<double>::infinity();

    for (const auto& pair : pairs) {
        double distance = calculateDistance(pair.first, pair.second);
        minDistance = min(minDistance, distance);
    }

    return minDistance;
}

double findMinDistanceOptimized(const vector<pair<Point, Point>>& pairs, double d) {
    double minDistance = numeric_limits<double>::infinity();
    unordered_map<int, unordered_map<int, vector<Point>>> grid;

    for (const auto& pair : pairs) {
        Point p1 = pair.first;
        Point p2 = pair.second;

        int gridX1 = static_cast<int>(p1.x / d);
        int gridY1 = static_cast<int>(p1.y / d);
        int gridX2 = static_cast<int>(p2.x / d);
        int gridY2 = static_cast<int>(p2.y / d);

        grid[gridX1][gridY1].push_back(p1);
        grid[gridX2][gridY2].push_back(p2);
    }

    for (const auto& pair : pairs) {
        Point p1 = pair.first;
        Point p2 = pair.second;

        int gridX = static_cast<int>(p1.x / d);
        int gridY = static_cast<int>(p1.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {
                    for (const auto& point : grid[i][j]) {
                        double distance = calculateDistance(p1, point);
                        minDistance = min(minDistance, distance);
                    }
                }
            }
        }
    }

    return minDistance;
}


int main() {
    int n = 100;
    vector<pair<Point, Point>> randomPoints = generateRandomPointPairs(n);

    double d = 0.1;/*numeric_limits<double>::max();

    for (const auto& pair : randomPoints) {
        double dist = calculateDistance(pair.first, pair.second);
        if (dist < d) {
            d = dist;
        }
    }*/


    double minDistance = findMinDistanceOptimized(randomPoints, d);

    cout << "La distancia mínima entre los puntos es: " << minDistance << endl;

    return 0;

}