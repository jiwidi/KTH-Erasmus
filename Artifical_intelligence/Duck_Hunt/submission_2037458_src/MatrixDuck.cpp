/* Matrix.cpp - Assignment 1 AI
 * Julia Guerrero Viu && Jaime Ferrando Huertas
 * Class matrix
 */

#include <iostream>
#include <vector>
#include "MatrixDuck.hpp"

using namespace std;

    Matrix::Matrix(){
        this->n=1;
        this->m=1;
        vector<double> v(m);
        for (int i=0; i<n; i++){
            matrix.push_back(v);
        }
    }

    Matrix::Matrix(const int n, const int m){
        this->n = n;
        this->m = m;
        vector<double> v(m);
        for (int i=0; i<n; i++){
            matrix.push_back(v);
        }
    }
    
    Matrix::Matrix(const int n, const int m, const double x){
        this->n = n;
        this->m = m;
        vector<double> v(m,x);
        for (int i=0; i<n; i++){
             matrix.push_back(v);
             for(int j=0; j<m; j++){
                matrix[i].push_back(x);
             }
        }
    }

    int Matrix::getn() const{
        return n;
    }

    int Matrix::getm() const{
        return m;
    }

    double Matrix::getelement(const int i, const int j) const{
        return matrix[i][j];
    }

    vector<double> Matrix::getvector (const int i) const{
        return matrix[i];
    }

    void Matrix::addelement (const double a, const int i, const int j){
        matrix[i][j] = a;
    }

    void Matrix::addvector (const vector<double>& v, const int i){
        matrix[i] = v;
    }

    string Matrix::print() const{
        string cad;
        cad+= to_string(n)+ " " + to_string(m);
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                cad+= " " + to_string(matrix[i][j]);
            }
        }
        cad+= "\n";
        return cad;
    }

    void Matrix::identity() {
        matrix.clear();
        
        for (int i=0; i<n; i++){
            vector<double> v(m,0);
            v[i]=1;
            matrix.push_back(v);
        }
    }
    
    void Matrix::resize(const int n, const int m){
        matrix.clear();
        this->n=n;
        this->m=m;
        vector<double> v(m);
        for (int i=0; i<n; i++){
            matrix.push_back(v);
        }
    }

    Matrix Matrix::transpose() const{
        Matrix trans=Matrix(getm(),getn());
        for(int i = 0; i < getn(); ++i)
            for(int j = 0; j < getm(); ++j)
            {
                trans.addelement(matrix[i][j],j,i);
            }
        return trans;

    }

    void Matrix::normalize() {
        for(int i=0; i<n; i++){
            double sum = 0;
            for(int j=0; j<m; j++){
                sum = sum + matrix[i][j];
            }
            for(int j=0; j<m; j++){
                matrix[i][j] *= 1/sum;            
            }
        }
    }

    void Matrix::shuffle() {
        matrix.clear();

        for(int i=0; i<n;i++){
            vector <double> v(m, (1/(double) m) );;
            double sum=1;
            for(int j=0;j<m-1;j++){
                double r1=((double) rand() / (RAND_MAX));
                double r2=((double) rand() / (RAND_MAX));
                double ep=0.1*(1/(double) m); 
                v[j]+=r1*ep-(r2*ep);
                sum=sum-v[j];

            }
            v[m-1]=sum;
            matrix.push_back(v);
        }
    }

    void Matrix::avoidzeros(){
        double epsilon = 0.0000001;
        for (int i=0; i<n; i++){
            for (int j=0; j<m; j++){
                if (matrix[i][j]==0){
                    matrix[i][j] = epsilon;                
                }
            }
        }
        this->normalize();
    }

    vector<double> Matrix::operator*(const vector<double>& v){
        vector<double> resul(n);
        if(m != v.size()){
            cerr << "Dimensions are not correct to multiply" << "m " << m << " vector" << v.size() << endl;
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

    Matrix Matrix::operator*(const Matrix& mat1) {
        Matrix resul(mat1.getm(),n);
        Matrix trans = mat1.transpose();
        for(int i=0; i<mat1.getm(); i++){
            vector<double> v = *this * (trans.getvector(i));
            resul.addvector(v,i);
        }
        return resul.transpose();
    }


