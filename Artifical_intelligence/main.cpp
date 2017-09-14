#include <iostream>
#include "Matrix.cpp"

int main(){
    Matrix a(4,4,0.3);
    a.print();
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            a.addelement(i+j, i,j);
        }    
    }
    a.print();
    a.normalize();
    a.print();

}
