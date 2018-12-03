#include <iostream>
using namespace std;

int main(){
    string text {"eu amo arroz"}, a {"arroz"}, b {"love"};
    if(text.find(a) != string::npos)
        cout << "achei " + a;
    if(text.find(b) != string::npos)
        cout << "achei " + b;
    return 0;
}

/* int main () {
    setlocale(LC_ALL, "portuguese");
    unsigned int cp = 1252;

    //Respaldo do codepage original da consola
    unsigned int cpIn  = GetConsoleCP();
    unsigned int cpOut = GetConsoleOutputCP(); 

    cout << "anãoção çãao õ" << endl;
    //Estabelecendo o codepage de saída para 1252(latin2).
    SetConsoleOutputCP(cp);
    cout << "anãoção çãao õ" << endl;

    //Estabelecendo o codepage de Entrada para 1252(latin2).
    SetConsoleCP(cp);
    cout << "anãoção çãao õ" << endl;

    SetConsoleCP(cpIn);
    SetConsoleOutputCP(cpOut);

    return 0;
} */


// OK
/* class Var{
public:
    int ** name = nullptr;
    
    Var(int ** name)
        : name(name){}

    void setAndress(int * p){
        *name = p;
    }
    void setNullptr(){
        *name = nullptr;
    }    
    void show(){
        cout << &**name;
    }
};

main(){
    int * i = nullptr;
    Var * var = new Var(&i);

    int * p = new int {10};

    var->setAndress(p);
    if(i != nullptr){
        cout << "nao sou mais nulo, main *i = ";
        cout << *i;
        cout << endl;
    }

    var->setNullptr();
    if(i == nullptr)
        cout << "agora sou nulo, main *i";

    return 0;
} */

// OK
/* class Var{
public:
    int ** name = nullptr;
    
    Var(int ** name)
        : name(name){}

    void setAndress(int * p){
        *name = p;
    }

    void show(){
        cout << &**name;
    }
};

main(){
    int * i = nullptr;
    Var * var = new Var(&i);

    int * p = new int {10};

    var->setAndress(p);

    cout << &*p;
    cout << endl;
    cout << &*i;
    cout << endl;
    var->show();

    return 0;
} */

// OK
/* class Var{
public:
    string ** name = nullptr;

    Var(string ** name)
        : name(name){}

    void morelove(){
        **name += " : i love u";
        cout << "Var **name += i love u\n";
    }
    void showValue(){
        cout << "Var **name = " + **name + "\n";
    }
};

main(){
    string * i = nullptr;
    Var * var = new Var(&i);

    i = new string{"diego"};

    cout << "main *i = " + *i  + "\n";
    var->showValue();

    var->morelove();
    cout << "main *i = " + *i  + "\n";
    var->showValue();

    return 0;
} */

// KO ** fucionando!
/* class Var{
public:
    int ** value = nullptr;

    Var(int ** val)
        : value(val){}

    void soma10(int * i){
        **value+=10;
        cout << "Var **value += 10\n";
    }
    void showValue(){
        cout << "Var **value : " + to_string(**value);
    }
};

main(){
    int * i = nullptr;
    Var * var = new Var(&i);

    i = new int{10};
    cout << "main *i : " + to_string(*i) + "\n";

    var->soma10(i);
    cout << "main *i : " + to_string(*i) + "\n";
    
    *i-=5;
    cout << "main *i -= 5\n";

    var->showValue();

    return 0;
} */

// OK
/* class Var{
public:
    int * value = nullptr;

    Var(int * val = nullptr)
        : value(val){}

    void soma10(int * i){
        *value+=10;
        cout << "Var *value += 10\n";
    }
    void showValue(){
        cout << "Var *value : " + to_string(*value);
    }
};

main(){
    int * i = new int {0};
    Var * var = new Var(i);

    cout << "main *i : " + to_string(*i) + "\n";

    var->soma10(i);
    cout << "main *i : " + to_string(*i) + "\n";
    
    *i-=5;
    cout << "main *i -= 5\n";

    var->showValue();

    return 0;
} */

// OK
/* void ponteiro(int * i){ *i+=2; }
main(){
    int * i = new int {10};
    ponteiro(i);
    cout << *i;
    return 0;
} */

// OK
/* void ponteiro(int * i){ *i+=2; }
main(){
    int a = 0;
    int* i = &a;
    ponteiro(i);
    cout << a;
    return 0;
} */

// crio variável, passo a referência, e
// dentro da função atribuo o endereço a um ponteiro
/* void ponteiro(int &i){
    int * p = &i;
    *p+=2;
}

main(){
    int i = 0;
    ponteiro(i);
    cout << i;
    return 0;
} */