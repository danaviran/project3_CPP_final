#include "GenericLinkedList.h"
#include <iostream>

int main() {
    
    LinkedList<int> l;

    // test for push
    for(int i=0; i<10; i++) l.push(i);

    // test erase
    l.erase(9); //first
    l.erase(5); // middle
    l.erase(0); // last
    
    // iterators
    std::cout << "Iterate & print, l: " << std::endl;
    for(auto item : l)
        std::cout << item << std::endl;
    
    // iterators with STL algorithm
    auto it = std::find(l.begin(),l.end(),3);
    if (it != l.end()) {
        std::cout << "found 3 in the list!" << std::endl;
    } else {
        std::cout << "no 3 in the list!" << std::endl;
    }

    // Copy constructor
    LinkedList<int> l2(l);
    std::cout << "Copy constructor, l2: " << std::endl;
    std::cout << l2 << std::endl;


    // Test operator =
    l.push(5);
    //LinkedList<int> l3;
    l2 = l;
    std::cout << "Operator=, l2:" << std::endl;
    std::cout << l2 << std::endl;

    // test T=string
    LinkedList<std::string> sl;
    sl.push("hi templates!");
    sl.push("string linked list");
    std::cout << "List of strings:\n";
    std::cout << sl;
    
    return 0;
}
