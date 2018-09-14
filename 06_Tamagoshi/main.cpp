#include <iostream>
#include <clocale>
#include "pet.hpp"
#include "controller.hpp"

using namespace std;

int main(){
    setlocale(LC_ALL,"Portuguese");
    Controller controller;
    controller.exec();

    return 0;
}