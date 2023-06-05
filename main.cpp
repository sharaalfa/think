#include <iostream>
#include "interval_map.cpp"
using namespace std;
int main() {
    interval_map<int, char> obj('A');
//    interval_map<int, char> interval_map('A');
    obj.assign(2, 3, 'B');
    std::cout << obj.operator[](5) << std::endl;
    return 0;
}
