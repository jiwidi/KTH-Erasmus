/* Matrix.hpp - Assignment 1 AI
 * Julia Guerrero Viu && Jaime Ferrando Huertas
 * Class matrix
 */


#include <iostream>
#include <vector>


#ifndef mymatrix_H
#define mymatrix_H


using namespace std;

class Matrix{
    vector <vector<double> > matrix;
    int n, m;

public:
    // Initialize the matrix with dimensions 1, 1 and not values inside
    Matrix();

    Matrix(const int n, const int m);
    
    // Initialize the matrix with dimensions n, m and the value x in each position
    Matrix(const int n, const int m, const double x);

    int getn() const;

    int getm() const;

    double getelement(const int i, const int j) const;

    vector<double> getvector (const int i) const;

    void addelement (const double a, const int i, const int j);

    void addvector (const vector<double>& v, const int i);

    string print() const;

    void identity();
    
    void resize(const int n, const int m);

    Matrix transpose() const;

    // normalize the matrix to make it row stocastic
    void normalize();
    
    // fill the matrix with random values and normalizes it
    void shuffle();
        
    // change the zeros for epsilon values and normalize the matrix again
    void avoidzeros();

    vector<double> operator*(const vector<double>& v);

    Matrix operator*(const Matrix& mat1);

};

#endif
