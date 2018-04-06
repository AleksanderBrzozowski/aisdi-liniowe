#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "Vector.h"
#include "LinkedList.h"

using namespace aisdi;

template<typename Func>
long long measureTime(Func f) {
    const auto start = std::clock();
    f();
    const auto end = std::clock();
    return end - start;
}

void fillLinkedList(LinkedList<int> &linkedList, int elements) {
    for (int i = 0; i < elements; ++i) {
        linkedList.append(i);
    }
}

void fillVector(Vector<int> &vector, int elements) {
    for (int i = 0; i < elements; ++i) {
        vector.append(i);
    }
}


struct statistics {
    long long vectorTime;
    long long linkedListTime;

    statistics(long long int vectorTime, long long int linkedListTime) : vectorTime(vectorTime),
                                                                         linkedListTime(linkedListTime) {}
};

void printTime(const statistics &statistics, int elements) {
    std::cout << "Vector time: " << statistics.vectorTime << ", Linked list time: "
              << statistics.linkedListTime << ", Elements: " << elements << std::endl;
}

template<typename VectorFunc, typename LinkedListFunc>
void performMeasureTime(VectorFunc vectorF, LinkedListFunc linkedListF, int elements) {
        Vector<int> vector;
        LinkedList<int> linkedList;

        fillVector(vector, elements);
        fillLinkedList(linkedList, elements);
        const statistics &statistics = {measureTime([&]() -> void { vectorF(vector); }),
                                        measureTime([&]() -> void { linkedListF(linkedList); })};
        printTime(statistics, elements);
}

void testBegin(Vector<int> elements) {
    std::cout << "<<Measure begin>>" << std::endl;
    for (const auto i: elements) {
        performMeasureTime(
                [&](Vector<int> &vector) -> void { vector.prepend(1); },
                [&](LinkedList<int> &linkedList) -> void { linkedList.prepend(1); },
                i
        );
    }
    std::cout << "<<End begin>>" << std::endl;
}


void testEnd(Vector<int> elements) {
    std::cout << "<<Measure end>>" << std::endl;
    for (const auto i: elements) {
        performMeasureTime(
                [&](Vector<int> &vector) -> void { vector.append(1); },
                [&](LinkedList<int> &linkedList) -> void { linkedList.append(1); },
                i
        );
    }
    std::cout << "<<End end>>" << std::endl;
}

void testGetFirst(Vector<int> elements) {
    std::cout << "<<Measure get first>>" << std::endl;
    for (const auto i: elements) {
        performMeasureTime(
                [&](Vector<int> &vector) -> void { *(vector.begin()); },
                [&](LinkedList<int> &linkedList) -> void { *(linkedList.begin()); },
                i
        );
    }
    std::cout << "<<End get first>>" << std::endl;
}

void testGetLast(Vector<int> elements) {
    std::cout << "<<Measure get last>>" << std::endl;
    for (const auto i: elements) {
        performMeasureTime(
                [&](Vector<int> &vector) -> void { *(--vector.end()); },
                [&](LinkedList<int> &linkedList) -> void { *(--linkedList.end()); },
                i
        );
    }
    std::cout << "<<End get last>>" << std::endl;
}

void testGetMiddle(Vector<int> elements) {
    std::cout << "<<Measure get middle>>" << std::endl;
    for (const auto i: elements) {
        performMeasureTime(
                [&](Vector<int> &vector) -> void { *(vector.begin() + i / 2); },
                [&](LinkedList<int> &linkedList) -> void { *(linkedList.begin() + i / 2); },
                i
        );
    }
    std::cout << "<<End get middle>>" << std::endl;
}

int main() {
    Vector<int> elements{10000, 100000, 1000000};
    testBegin(elements);
    testEnd(elements);
    testGetFirst(elements);
    testGetLast(elements);
    testGetMiddle(elements);
    return 0;
}

