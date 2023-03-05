#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "vector.hpp"

using std::to_string;


template <class T>
std::string to_string(const std::vector<T> &v) {
    using std::to_string;  // allow custom implementations
    std::string result;
    result += "[";
    for (std::size_t i = 0; i < v.size(); i++) {
        if (i) result += ", ";
        result += to_string(v[i]);
    }
    result += "]";
    return result;
}

int main() {
    Vector<int> v;

    std::cout << v.size() << " " << to_string(v) << "\n";

    v.push_back(6);
    std::cout << to_string(v) << "\n";
    v.push_back(78);
    std::cout << to_string(v) << "\n";
    v.push_back(72);
    std::cout << to_string(v) << "\n";
    v.push_back(7234);
    std::cout << to_string(v) << "\n";
    v.push_back(34);
    std::cout << to_string(v) << "\n";

    std::cout << "\n";
    Vector<Vector<int>> vv(5);
    std::cout << "vv " << vv.size() << " " << to_string(vv) << "\n";
    for (std::size_t i = 0; i < vv.size(); i++) {
        if (i > 0) vv[i].push_back(6);
        if (i > 1) vv[i].push_back(78);
        if (i > 2) vv[i].push_back(72);
        if (i > 3) vv[i].push_back(7234);
        if (i > 4) vv[i].push_back(34);
        std::cout << to_string(vv[i]) << "\n";
    }
    std::cout << "vv " << vv.size() << " " << to_string(vv) << "\n";

    std::cout << "\n";
    Vector<Vector<int>> vv1;
    vv1.reserve(5);
    std::cout << "vv1 " << vv1.size() << " " << to_string(vv1) << "\n";
    for (std::size_t i = 0; i < 5; i++) {
        vv1.push_back(Vector<int>());
        vv1[i].reserve(5);

        if (i > 0) vv1[i].push_back(6);
        if (i > 1) vv1[i].push_back(78);
        if (i > 2) vv1[i].push_back(72);
        if (i > 3) vv1[i].push_back(7234);
        if (i > 4) vv1[i].push_back(34);
        std::cout << to_string(vv1[i]) << "\n";
    }
    std::cout << "vv1 " << vv1.size() << " " << to_string(vv1) << "\n";
}
