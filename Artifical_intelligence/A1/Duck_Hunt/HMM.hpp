/* HMM.hpp - Assignment 1 AI
 * Julia Guerrero Viu && Jaime Ferrando Huertas
 * Class HMM which represents a model of a Hidden Markov Model
 */

#include <iostream>
#include <vector>
#include <tuple>
#include "MatrixDuck.hpp"
#include <algorithm>

#ifndef myhmm_H
#define myhmm_H

using namespace std;

class HMM{
    Matrix A;
    Matrix B;
    vector<double> pi;

public:
    HMM(){}
    // Initialize a new HMM randomly
    HMM(const int st, const int obs);

    HMM(const Matrix& A, const Matrix& B, const vector<double>& pi);

    int getst() const;

    int getobs() const;

    Matrix getA() const;
    
    Matrix getB() const;

    // return the probability of the observation sequence
    double hmm1(const vector<int>& obs) const;

    // return the values of vector alpha normalized in each time step between 0-it using forward algorithm and the vector of the scales used
    tuple<Matrix,vector<double>> alphapass(const vector<int>& obs, const int it) const;

    // return the values of vector beta normalized in each time step between 0-it using backward algorithm given the vector c of scales
    Matrix betapass (const vector<int>& obs, const int it, const vector<double>& c) const;
    
    // return the values of gamma and Digamma normalized in each time step between 0-it 
    tuple<vector<Matrix>,Matrix> gammaDigamma (const vector<int>& obs, const int it, const vector<double>& c, const Matrix& alpha, const Matrix& beta) const;

    // return the gamma vector in time step it
    vector<double> gamma(const vector<int>&obs, const int it) const;

    // Trains and reestimates the model using Baum-Welch algorithm given the observation sequence obs
    void hmm3(const vector<int>& obs);

    string print();
    
    int calculateState(const vector<int>& obs);

    tuple<int,double> nextObs(const vector<int>& obs);

};

#endif
