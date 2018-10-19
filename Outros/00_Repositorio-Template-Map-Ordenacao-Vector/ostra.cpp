#include <iostream>
using namespace std;

template<typename T>
struct Ostra{
    T value;
    Ostra(T value = 0){
        this->value = value;
    }
    T getValue(){
        return value;
    }
};

int main(){
    Ostra<int> ostra_vet_int(5);
    cout << ostra_vet_int.getValue() << endl;
    return 0;
}