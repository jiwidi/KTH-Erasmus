/* HMM0 for A1
 * Jaime Ferrando & Julia Guerrero
 */

#include <iostream>
#include <vector>

using namespace std;

class Matrix{
    vector <vector<double>> matrix;
    int n, m;

public:
    Matrix(int n, int m){
        this->n = n;
        this->m = m;
        vector<double> v;
        matrix.push_back(v);
    }
    
    int getn(){
        return n;   
    } 
    
    int getm(){
        return m;    
    }    

    void add (double a){
        bool done = false;        
        for(int i=0; (i < n) && (not done); i++){
            if(matrix[i].size() < m){
                matrix[i].push_back(a);
                done = true;
            }
            else {
                vector<double> v;
                matrix.push_back(v);
            }
        }
        if (not done){
            cerr << "The matrix is already full, not possible to add elements";
        }
    }

    void print(){
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                cout << matrix[i][j] << " ";
            }
            cout << '\n';
        }
    }

//    vector<double> operator*(const vector<double>& v){

//    }

//    Matrix operator*(const Matrix& mat1) {
//        int n1= mat1.getn();
//        int m1= mat1.getm();
//    }

    
};

int main(){
    
    int na, ma;
    cin >> na >> ma;

    Matrix A(na,ma);

    double element;
    
    for(int i=0; i<(na*ma); i++){
        cin >> element;
        A.add(element);
    }

    A.print();

    int nb, mb;
    cin >> nb >> mb;

    Matrix B(nb,mb);
    
    for(int i=0; i<(nb*mb); i++){
        cin >> element;
        B.add(element);
    }

    B.print();

    int npi, mpi;
    cin >> npi >> mpi;

    Matrix pi(npi,mpi);

    for(int i=0; i<(npi*mpi); i++){
        cin >> element;
        pi.add(element);
    }

    pi.print();
    
    return 0;
}
