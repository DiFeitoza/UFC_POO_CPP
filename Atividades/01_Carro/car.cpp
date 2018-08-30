#include <cstdio>
#include <iostream>
#include <sstream>

using namespace std;

struct Car{
    int pass;
    int passMax;
    float gas;
    float gasMax;
    float km;

    bool in(){
        if (pass==2) return false;
        pass+=1;
        return true;
    }

    bool out(){
        if (pass<1) return false;
        pass-=1;
        return true;
    }

    bool fuel(float value){
        if (gas==10){
            return false;
        }
        else if ((gas+value)>=10){
            gas=10;
            return true;
        }
        gas=gas+value;
        return true;
    }

    bool drive(float distance, string &scout){
        if (gas==0){
            scout = "\tfail: carro sem combustivel\n";
            return false;
        }
        else if (pass<1){
            scout = "\tfail: nao ha ninguem no carro\n";
            return false;
        }
        else if (distance<=0){
            scout = "\tfail: seu veiculo não percorre distancias nulas ou negativas\n";
            return false;
        }
        else if (distance>(gas*10)){
            scout = "\tfail: gasolina insuficiente\n";
            return false;
        }
        
        km+=distance;
        gas-=(distance/10);
        
        return true;
    }

    string show(){
        stringstream saida;
        saida
        << "\tpass: " << pass << ", "
        << "gas: " << gas << ", "
        << "km: " << km << "\n";

        return saida.str();
    }
};

int main(){
    string op;

    Car car = {0,2,0,10,0};

    while(true){
        cin >> op;

        if (op == "show"){
            cout << car.show();
        }
        else if (op == "in"){
            if(car.in()) cout << "\tdone\n";
            else cout << "\tfail: limite de pessoas atingido\n";;
        }
        else if (op == "out"){
            if(car.out()) cout << "\tdone\n";
            else cout << "\tfail: nao ha ninguem no carro\n";
        }
        else if (op == "fuel"){
            int value;
            cin >> value;
            if(car.fuel(value)) cout << "\tdone\n";
            else cout << "\tfail: tanque cheio\n";
        }
        else if (op == "drive"){
            string scout;
            float distance;
            cin >> distance;
            if(car.drive(distance, scout)){
                cout << "\tdone\n";
            } else {
                cout << scout;
            }
        }
        else if (op == "fim"){
            break;
        }
    }

    return 0;
}