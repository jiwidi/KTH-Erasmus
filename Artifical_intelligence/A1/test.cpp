#include "MatrixDuck.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
    Matrix A(5,5);
    cout << A.print();
    A.identity();
    cout << A.print();

    // srand(time(NULL));
    // vector<double> pi;
    // double ep=0.005;
    // double sum=1;
    // for (int i=0;i<9-1;i++){
    //     double r1=((double) rand() / (RAND_MAX));
    //     double r2=((double) rand() / (RAND_MAX));
    //     double v=1/(double) 9;
    //     v=v+r1*ep-(r2*ep);
    //     sum= sum-v;
    //     pi.push_back(v);
    // }
    // pi.push_back(sum);

    // for(int i=0;i<9;i++){
    //     cout << pi[i] << "\n";
    // }
    A.shuffle();
    cout << A.print();



    return 0;
}
