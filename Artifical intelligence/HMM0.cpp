/* HMM0 for A1
 * Jaime Ferrando & Julia Guerrero
 */

#include <iostream>
#include <vector>

using namespace std;

class Matrix{
    vector <vector<double> > matrix;
    int n, m;

public:
    Matrix(int n, int m){
        this->n = n;
        this->m = m;
        vector<double> v;
        matrix.push_back(v);
    }
    
    vector <vector<double> > getmatrix() const{
        return matrix;
    }    

    int getn() const{
        return n;   
    } 
    
    int getm() const{
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

    Matrix transpose(){
        Matrix trans=Matrix(getm(),getn());
        for(int i = 0; i < getn(); ++i)
            for(int j = 0; j < getm(); ++j)
            {
                trans.add(matrix[j][i]);
            }
        return trans;

    }
    
    vector<double> operator*(const vector<double>& v){
        vector<double> resul(m,0);
        if(m != v.size()){
            cerr << "The dimensions are not correct to multiply";
            return v;
        }
        else{
            for (int i=0; i<n; i++){
                for (int j=0; j<m; j++){
                    resul[i] = resul[i] + matrix[i][j] * v[j];
                }
             }
        }
        return resul;
    }

    Matrix operator*(Matrix mat1) {
        Matrix resul(this->n,mat1.getm());
        Matrix trans = mat1.transpose();
        vector<double> v;
        for(int i=0; i<m; i++){
            v = *this *(trans.getmatrix())[i];
            for(int j=0; j<mat1.getn(); j++){
                resul.add(v[j]);
            }
        }
        return resul.transpose();
    }

    
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

    Matrix m = A*B;
    m.print();
    
    return 0;
}
