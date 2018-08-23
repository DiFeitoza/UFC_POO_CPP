#include <stdio.h>

#include <iostream>
using namespace std;

struct Car{
    int pass;
    int passMax;
    float gas;
    float gasMax;
    float km;

    void in(){
        if (pass==2){
            cout << "\tfail: limite de pessoas atingido\n";
        }
        else {
            pass+=1;
            cout << "\tdone\n";
        }
    }

    void out(){
        if (pass<1){
            cout << "\tfail: nao ha ninguem no carro\n";
        }
        else{
            pass-=1;
            cout << "\tdone\n";
        }
    }

    void fuel(float value){
        if (gas==10){
            cout << "\tfail: tanque cheio\n";
        }
        else if ((gas+value)>=10){
            gas=10;
            cout << "\tdone\n";
        }
        else {
            gas=gas+value;
            cout << "\tdone\n";
        }
    }

    void drive(float distance){
        if (gas==0){
            cout << "\tfail: carro sem combustivel\n";
        }
        else if (pass<1){
            cout << "\tfail: nao ha ninguem no carro\n";
        }
        else if (distance<=0){
            cout << "\tfail: seu veiculo não percorre distancias nulas ou negativas\n";
        }
        else if (distance>(gas*10)){
            cout << "\tfail: gasolina insuficiente\n";
        }
        else {
            km+=distance;
            gas-=(distance/10);
            cout << "\tdone\n";
        }
    }

    void show(){
        cout
        << "\tpass: " << pass << ", "
        << "gas: " << gas << ", "
        << "km: " << km << "\n";
    }
};

int main(){
    string op;

    Car car = {0,2,0,10,0};

    while(true){
        cin >> op;

        if (op == "show"){
            car.show();
        }
        else if (op == "in"){
            car.in();
        }
        else if (op == "out"){
            car.out();
        }
        else if (op == "fuel"){
            int value;
            cin >> value;
            car.fuel(value);
        }
        else if (op == "drive"){
            float distance;
            cin >> distance;
            car.drive(distance);
        }
        else if (op == "fim"){
            break;
        }
    }

    return 0;
}