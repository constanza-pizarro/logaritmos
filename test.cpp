#include "d_a_c.h"


int main() {
    // Ejemplo de uso
    vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}};
    double closestDistance = divide_and_conquer(points);

    cout << "La distancia más cercana entre puntos es: " << closestDistance << endl;

    return 0;
}