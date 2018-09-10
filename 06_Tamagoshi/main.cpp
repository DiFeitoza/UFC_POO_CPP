#include <iostream>
#include "pet.hpp"

using namespace std;

int main(){
    Pet p("Bixanuh", 15, 10, 300);
    p.play();
    cout << p.toString() << endl;
    p.play();
    cout << p.toString() << endl;
    p.play();
    cout << p.toString() << endl;
    while(true)
        p.play();

    return 0;
}