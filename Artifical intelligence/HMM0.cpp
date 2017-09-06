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
    Matrix(const int n, const int m){
        this->n = n;
        this->m = m;
        vector<double> v(m,0);
        for (int i=0; i<n; i++){
             matrix.push_back(v);
        }
    }

    int getn() const{
        return n;   
    } 
    
    int getm() const{
        return m;    
    }
    
    double getelement(const int i, const int j) const{
        return matrix[i][j];
    } 

    vector<double> getvector (const int i) const{
        return matrix[i];
    } 

    void addelement (const double a, const int i, const int j){
        matrix[i][j] = a;
    }
    
    void addvector (const vector<double>& v, const int i){
        matrix[i] = v;
    }

    void print() const{
        cout << n << " " << m;
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                cout << " " << matrix[i][j];
            }
        }
    }

    Matrix transpose() const{
        Matrix trans=Matrix(getm(),getn());
        for(int i = 0; i < getn(); ++i)
            for(int j = 0; j < getm(); ++j)
            {
                trans.addelement(matrix[i][j],j,i);
            }
        return trans;

    }
    
    vector<double> operator*(const vector<double>& v){
        vector<double> resul(m,0);
        if(m != v.size()){
            cerr << "Dimensions are not correct to multiply";
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

    Matrix operator*(const Matrix& mat1) {
        Matrix resul(mat1.getm(),n);
        Matrix trans = mat1.transpose();
        for(int i=0; i<mat1.getm(); i++){
            vector<double> v = *this * (trans.getvector(i));
            resul.addvector(v,i);
        }
        return resul.transpose();
    }

    
};

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
