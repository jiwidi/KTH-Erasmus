#include <iostream>
#include <vector>


#ifndef mymatrix_H
#define mymatrix_H


using namespace std;

class Matrix{
    vector <vector<double> > matrix;
    int n, m;

public:
    Matrix(const int n, const int m);
    
    Matrix(const int n, const int m, const double x);
    
    ~Matrix();

    int getn() const;

    int getm() const;

    double getelement(const int i, const int j) const;

    vector<double> getvector (const int i) const;

    void addelement (const double a, const int i, const int j);

    void addvector (const vector<double>& v, const int i);

    string print() const;

    void identity();

    Matrix transpose() const;

    void normalize();
    
    void shuffle();
        
    void avoidzeros();

    vector<double> operator*(const vector<double>& v);

    Matrix operator*(const Matrix& mat1);

};

#endif
