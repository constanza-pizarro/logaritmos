#include "../div_and_con.h"

int main() {
    vector<Point> arr = {{1, 1.5}, {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {1, 1.2}};
    double min_dist = divide_and_conquer(arr);

    cout << "La distancia mínima entre pares es: " << min_dist << endl;

    return 0;
}
