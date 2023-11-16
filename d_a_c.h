#include <stdio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <ctime> // para clock_t, clock(), CLOCKS_PER_SEC
#include <cmath> // para std::abs
//#include <limits>

using namespace std;

struct Point {
    double x, y;
};

bool compare_x(const Point& a, const Point& b) {
    return a.x < b.x;
}

bool compare_y(const Point& a, const Point& b) {
    return a.y < b.y;
}

double euclidian_distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

/*
double bruteForce(const vector<Point>& points, int left, int right) {
    double minDistance = numeric_limits<double>::infinity();
    for (int i = left; i <= right; ++i) {
        for (int j = i + 1; j <= right; ++j) {
            minDistance = min(minDistance, euclidian_distance(points[i], points[j]));
        }
    }
    return minDistance;
}
*/

double findClosestPair(const vector<Point>& points, int left, int right) {
    /*
    if (right - left <= 2) {
        return bruteForce(points, left, right); // revisar
    }
    */
    if (right - left <= 2) { 
        double minDistance = numeric_limits<double>::infinity();
        for (int i = left; i <= right; ++i) {
            for (int j = i + 1; j <= right; ++j) {
                minDistance = min(minDistance, euclidian_distance(points[i], points[j]));
            }
        }
        return minDistance;
    }

    int mid = (left + right) / 2;
    double leftMin = findClosestPair(points, left, mid);
    double rightMin = findClosestPair(points, mid + 1, right);

    double minDistance = min(leftMin, rightMin);

    vector<Point> strip;
    for (int i = left; i <= right; ++i) {
        if (abs(points[i].x - points[mid].x) < minDistance) {
            strip.push_back(points[i]);
        }
    }

    sort(strip.begin(), strip.end(), compare_y);

    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size() && strip[j].y - strip[i].y < minDistance; ++j) {
            minDistance = min(minDistance, euclidian_distance(strip[i], strip[j]));
        }
    }

    return minDistance;
}

double divide_and_conquer(const vector<Point>& arr) {
    vector<Point> sorted_by_x = arr;
    sort(sorted_by_x.begin(), sorted_by_x.end(), compare_x);
    return findClosestPair(sorted_by_x, 0, sorted_by_x.size() - 1);
}