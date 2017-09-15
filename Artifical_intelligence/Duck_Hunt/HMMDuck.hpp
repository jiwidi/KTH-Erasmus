#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <cmath>
#include "MatrixDuck.hpp"

using namespace std;

// ------------------------- HMM1 -----------------------

vector <double> elWise(const vector<double>& v1,const vector<double>& v2);

vector <double> calcAlpha(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs);

// ------------------------- HMM3 -----------------------

tuple<Matrix,vector<double>> alphapass(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int it);


Matrix betapass (const Matrix& A, const Matrix& B, const vector<int>& obs, const int it, const vector<double>& c);


tuple<vector<Matrix>,Matrix> gammaDigamma (const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs, const int it, const vector<double>& c, const Matrix& alpha, const Matrix& beta);


tuple<Matrix,Matrix,vector<double>> reestimation(const vector<Matrix>& digamma, const Matrix& gamma, const vector<int>& obs, const int it, const int N, const int M);


tuple<Matrix,Matrix,vector<double>> hmm3(const Matrix& A, const Matrix& B, const vector<double>& pi, const vector<int>& obs);
