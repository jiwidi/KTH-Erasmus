from cvxopt.solvers import qp
from cvxopt.base import matrix
import numpy, pylab, random, math

LINEAR = 0
POLYNOMIAL = 1
RADIAL_BASIS = 2
SIGMOID = 3
EPSILON = 1.0e-5
POWER = 3
OMEGA = 2
DELTA = 0.1
K = 0.05
C = 20



def genDatapoints():
    numpy.random.seed(100)
    classA = [(random.normalvariate(-1.5, 1.0), random.normalvariate(0.5, 1.0), 1.0) for i in range(5)] + \
             [(random.normalvariate(1.5, 1.0), random.normalvariate(0.5, 1.0), 1.0) for i in range(5)]
    classB = [(random.normalvariate(0.0, 0.5), random.normalvariate(-0.5, 0.5), 1.0) for i in range(10)]
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB

# Linear kernel trick. Can only produce a linear boundary.
def linear_kernel(x, y):
    return numpy.dot(x,y) + 1

# Polynomial kernel trick that produces boundaries that are not a linear plane.
def polynomial_kernel(x, y):
    return linear_kernel(x, y)**POWER

# The Radial Basis function Kernel trick. OMEGA determines the smoothness.
def radial_basis_function_kernel(x, y):
    diff = numpy.subtract(x, y)
    return math.exp(-(numpy.dot(diff, diff))/(2*OMEGA**2))

# Sigmoid kernel trick. K should be around 1/N and delta should not be a large
# negative.
def sigmoid_kernel(x, y):
    return numpy.tanh(numpy.dot(numpy.multiply(K, x) ,y) + DELTA)


def kernel(x, y, kernel_trick):
    if kernel_trick == LINEAR:
        return linear_kernel(x, y)
    elif kernel_trick == POLYNOMIAL:
        return polynomial_kernel(x, y)
    elif kernel_trick == RADIAL_BASIS:
        return radial_basis_function_kernel(x, y)
    elif kernel_trick == SIGMOID:
        return sigmoid_kernel(x, y)
    else:
        return 0

def P(datapoints, kernel_trick):
    N = len(datapoints)
    P = numpy.empty(shape = (N, N))
    for i in range(N):
        for j in range(N):
            P[i][j] = datapoints[i][2] * datapoints[j][2] * kernel(datapoints[i][0:2], datapoints[j][0:2], kernel_trick)
    return P

def q(n):
    return [-1.0 for i in range(n)]

def h(n):
    h = numpy.zeros(shape = (2*n))
    for i in range(n):
        h[i+n] = C
    return h

def G(n):
    mat = numpy.zeros(shape = (2*n, n))
    for i in range(n):
        mat[i][i] = -1
        mat[i+n][i] = 1
    return mat
def getAlpha(P, q, G, h):
    r = qp(matrix(P), matrix(q), matrix(G), matrix(h))
    alpha = list(r['x'])
    return alpha   

def removeZ(datapoints,alpha):
    result = []
    for v1,v2 in zip(alpha, datapoints):
        if abs(v1) > EPSILON:
            result.append((v1,v2))
    return result

def getSupportVectors(datapoints,kernel_trick):
    n=len(datapoints)
    alpha=getAlpha(P(datapoints,kernel_trick),q(n),G(n),h(n))
    return removeZ(datapoints,alpha)

#Train its the result of removeZ
def indicator(supportVectors, x, y, kernel_trick):
    sum = 0
    for alpha, datapoint in supportVectors:
        sum += alpha*datapoint[2]*kernel((x, y), datapoint[0:2],kernel_trick)
    return sum

def kernel_name(kernel_trick):
    if kernel_trick == LINEAR:
        return 'Linear kernel'
    elif kernel_trick == POLYNOMIAL:
        return 'Polynomial kernel'
    elif kernel_trick == RADIAL_BASIS:
        return 'Radial Basis Function kernel'
    elif kernel_trick == SIGMOID:
        return 'Sigmoid kernel'
    else:
        return 'Fail'


def main():
    
    # Generate the datapoints and plot them.
    datapoints,classA, classB = genDatapoints()

    kernels = [LINEAR, POLYNOMIAL, RADIAL_BASIS, SIGMOID]
    for k in kernels:
        pylab.figure()
        pylab.title(kernel_name(k))
        pylab.plot([p[0] for p in classA],
                    [p[1] for p in classA],
                    'bo')
        pylab.plot([p[0] for p in classB],
                    [p[1] for p in classB],
                    'ro')

        t= getSupportVectors(datapoints, k)

        # Plot the decision boundaries.
        xr=numpy.arange(-4, 4, 0.05)
        yr=numpy.arange(-4, 4, 0.05)
        grid=matrix([[indicator(t, x, y, k) for y in yr] for x in xr])
        pylab.contour(xr, yr, grid, (-1.0, 0.0, 1.0), colors=('red', 'black', 'blue'), linewidths=(1, 3, 1))
    pylab.show()

if __name__ == '__main__':
    main()