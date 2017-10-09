from cvxopt.solvers import qp
from cvxopt.base import matrix
import numpy, pylab, random, math

LINEAR = 0
POLYNOMIAL = 1
RADIAL_BASIS = 2
SIGMOID = 3
EPSILON = 1.0e-5
BIAS = 1
POWER = 2
SIGMA = 1
DELTA = -0.1
K = 0.05
C = 20
SLACK = False


def genSpiral():
    classA = [ [0, 0, 1.0], [1, 0.5, 1.0], [1.5,0,1.0], [1,-1,1.0], [0,-1.5,1.0], [-1,-1.2,1.0], [-2,0,1.0], [-2,1,1.0], [0,2.8,1.0], [0.4,0.4,1.0], [-1.5,-0.7,1.0], [-1.1,2,1.0], [1,3,1.0] ]
    classB = [ [-0.5,-0.5,-1.0], [-1,0,-1.0], [-1,1,-1.0], [0,1.5,-1.0], [1,1.9,-1.0],[2,1.5,-1.0], [2.5,0,-1.0], [2,-1.5,-1.0], [1.5,-2,-1.0], [0,-3,-1.0], [-2,-2.5,-1.0], [-1,-2.88,-1.0], [0.8,-2.6,-1.0] ]
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB
    
def genCross():
    classA = [((i-4), (i-4), 1.0) for i in range(8)]
    classB = [((i-3.5), (3.5-i), -1.0) for i in range(8)]
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB
    
def genXOR():
    classA = [(random.normalvariate(-2, 0.75),random.normalvariate(-2, 0.75),1.0)
        for i in range(10)] + \
        [(random.normalvariate(2, 0.75), random.normalvariate(2, 0.75),1.0)
        for i in range(10)]
    classB = [(random.normalvariate(-2, 0.75),random.normalvariate(2, 0.75),-1.0)
        for i in range(10)] + \
        [(random.normalvariate(2, 0.75), random.normalvariate(-2, 0.75),-1.0)
        for i in range(10)]
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB
    
def genCircle():
    classA = [(random.normalvariate(0, 1),random.normalvariate(0, 1), 1.0)
        for i in range(20)]
    classB = [( 3*numpy.cos(t*numpy.pi/10), 3*numpy.sin(t*numpy.pi/10), -1.0)
        for t in range(20)]
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB
    
def genDatapoints():
    #classA = [(random.normalvariate(-2.5, 1.0), random.normalvariate(1.6, 0.8), 1.0)
    #            for i in range(5)] + [(random.normalvariate(2.5, 0.8), 
    #            random.normalvariate(2.0, 1.5), 1.0) for i in range(5)]
    #classB = [(random.normalvariate(0.0, 1.0), random.normalvariate(-1.2, 1.4), -1.0)
    #            for i in range(10)]
    #numpy.random.seed(25) good sample
    classA = [(random.normalvariate(-1.5, 1),random.normalvariate(0.5, 1),1.0)
        for i in range(5)] + \
        [(random.normalvariate(1.5, 1), random.normalvariate(0.5, 1),1.0)
        for i in range(5)]
    classB = [(random.normalvariate(0.0, 0.5), random.normalvariate(0, 0.5), -1.0)
        for i in range(10)]
    # Statement
    '''
    classA = [(random.normalvariate(-1.5, 1),random.normalvariate(0.5, 1),1.0)
        for i in range(5)] + \
        [(random.normalvariate(1.5, 1), random.normalvariate(0.5, 1),1.0)
        for i in range(5)]
    classB = [(random.normalvariate(0.0, 0.5), random.normalvariate(-0.5, 0.5), -1.0)
        for i in range(10)]
    '''
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB

# Linear kernel trick. Can only produce a linear boundary.
def linear_kernel(x, y):
    return numpy.dot(x,y) + BIAS

# Polynomial kernel trick that produces boundaries that are not a linear plane.
def polynomial_kernel(x, y):
    return linear_kernel(x, y)**POWER

# The Radial Basis function Kernel trick. SIGMA determines the smoothness.
def radial_basis_function_kernel(x, y):
    diff = numpy.subtract(x, y)
    return math.exp(-(numpy.dot(diff, diff))/(2*SIGMA**2))

# Sigmoid kernel trick. K should be around 1/N and delta should not be a large
# negative.
def sigmoid_kernel(x, y):
    return numpy.tanh(numpy.dot(numpy.multiply(K, x) ,y) - DELTA)


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
    if SLACK:
        h = numpy.zeros(shape = (2*n))
        for i in range(n):
            h[i+n] = C
    else:
        h = numpy.zeros(shape = n)
    return h

def G(n):
    if SLACK:
        mat = numpy.zeros(shape = (2*n, n))
        for i in range(n):
            mat[i][i] = -1
            mat[i+n][i] = 1
    else:
        mat = numpy.zeros(shape = (n, n))
        for i in range(n):
            mat[i][i] = -1
    return mat

def getAlpha(P, q, G, h):
    r = qp(matrix(P), matrix(q), matrix(G), matrix(h), kktsolver='ldl')
    alpha = list(r['x'])
    return alpha

def removeZ(datapoints,alpha):
    result = []
    for v1,v2 in zip(alpha, datapoints):
        if abs(v1) > EPSILON:
            result.append((v1,v2))
    return result

def train(datapoints,kernel_trick):
    n=len(datapoints)
    alpha=getAlpha(P(datapoints,kernel_trick),q(n),G(n),h(n))
    return removeZ(datapoints,alpha)

#Train its the result of removeZ
def indicator(train, x, y, kernel_trick):
    sum = 0
    for alpha, datapoint in train:
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
    datapoints, classA, classB = genCircle() #genCross() #genSpiral() #genDatapoints()

    kernels = [LINEAR, POLYNOMIAL, RADIAL_BASIS, SIGMOID]
    for k in kernels:
        pylab.figure()
        pylab.title(kernel_name(k))
        
        # Plot the datapoints
        pylab.plot([p[0] for p in classA],
                    [p[1] for p in classA],
                    'bo')
        pylab.plot([p[0] for p in classB],
                    [p[1] for p in classB],
                    'ro')

        t= train(datapoints, k)
        
        # Plot the support vectors, i.e. the datapoints with non-zero alpha
        for alpha, d in t:
            if d[2] == 1:
                pylab.plot(d[0], d[1], 'bH', markersize=10)
            else:
                pylab.plot(d[0], d[1], 'rH', markersize=10)

        # Plot the decision boundaries.
        xr=numpy.arange(-4, 4, 0.05)
        yr=numpy.arange(-4, 4, 0.05)
        grid=matrix([[indicator(t, x, y, k) for y in yr] for x in xr])
        pylab.contour(xr, yr, grid, (-1.0, 0.0, 1.0), colors=('red', 'black', 'blue'), linewidths=(1, 3, 1))
    pylab.show()

if __name__ == '__main__':
    main()