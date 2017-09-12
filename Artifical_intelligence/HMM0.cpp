/* HMM0 for A1
 * Jaime Ferrando & Julia Guerrero
 */

#include <iostream>
#include <vector>
#include "atrix.cpp"

using namespace std;



int main(){

    int na, ma;
    cin >> na >> ma;

    Matrix A(na,ma);

    double element;

    for(int i=0; i<na; i++){
        for (int j=0; j<ma; j++){
            cin >> element;
            A.addelement(element, i, j);
        }
    }

    int nb, mb;
    cin >> nb >> mb;

    Matrix B(nb,mb);

    for(int i=0; i<nb; i++){
        for (int j=0; j<mb; j++){
            cin >> element;
            B.addelement(element, i, j);
        }
    }

    int npi, mpi;
    cin >> npi >> mpi;

    Matrix pi(npi,mpi);

    for(int i=0; i<npi; i++){
        for (int j=0; j<mpi; j++){
            cin >> element;
            pi.addelement(element, i, j);
        }
    }

    Matrix resul = (pi*A)*B;
    resul.print();

    return 0;
}
