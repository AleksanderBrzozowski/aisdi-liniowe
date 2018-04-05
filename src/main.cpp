#include <string>
#include <iostream>
#include <algorithm>

#include "Vector.h"
#include "LinkedList.h"

int main() {
        aisdi::Vector<int> coll = {};
        aisdi::Vector<int> ot = { 100, 200, 300, 400 };
        ot = std::move(coll);
        std::cout << "true";
//        std::for_each(other.cbegin(), other.cend(), [&](const int item) { std::cout << item << std::endl; });

    {
        aisdi::LinkedList<int> collection = {};
        aisdi::LinkedList<int> other = { 100, 200, 300, 400 };
        other = std::move(collection);
        std::cout << "false";
//        std::for_each(other.cbegin(), other.cend(), [&](const int item) { std::cout << item << std::endl; });
    }

//    auto it = collection.begin();

//    const auto foo = *it;
//    const auto b = (++it == collection.cend());
//    std::for_each(collection.cbegin(), collection.cend(), [&](const int item) { std::cout << item << std::endl; });
//    aisdi::LinkedList<int> foo2(foo);
//    aisdi::Vector<int> foo = {1, 2, 3};
//    std::for_each(foo.begin(), foo.end(), [&](const int &e) { std::cout << e << std::endl; });
//    {
//        aisdi::Vector<int> foo3(std::move(foo));
//    }

//    std::for_each(foo.begin(), foo.end(), [&](const int &e) { std::cout << e << std::endl; });
//    std::for_each(foo2.begin(), foo2.end(), [&](const int &e) { std::cout << e << std::endl; });
    return 0;
}
