from cvxopt.solvers import qp
from cvxopt.base import matrix
import random, pylab, math
import numpy as np
import numpy

# todo: 1. Make it work with sigmoid!

classA = []
classB = []
data = []
epsilon = 0.0001

def get_data():
    global classA, classB, data
    np.random.seed(100)
    classA = [(random.normalvariate(-1.5, 1.0), random.normalvariate(0.5, 1.0), 1.0) for i in range(5)] + \
             [(random.normalvariate(1.5, 1.0), random.normalvariate(0.5, 1.0), 1.0) for i in range(5)]
    classB = [(random.normalvariate(0.0, 0.5), random.normalvariate(-0.5, 0.5), 1.0) for i in range(10)]
    data = classA + classB
    random.shuffle(data)
    return data,classA,classB

def scalar_product(x, y, dim):
    product = 0.0
    for i in range(dim):
        product += (x[i] * y[i])
    return product

def linear_kernel(x, y):
    return np.dot(x,y)+1

def poly_kernel(x, y, degree):
    poly = math.pow(linear_kernel(x, y), degree)
    return poly

def rbf_kernel(x, y, sigma):
    i = (x[0] - y[0], x[1] - y[1])
    rbf = math.exp(-1 * (scalar_product(i, i, 2) / (2 * sigma * sigma)))
    return rbf

def sigmoid_kernel(x, y, k, delta):
    sigmoid = math.tanh(scalar_product(x, y, 2) * k - delta)
    return sigmoid

def q_vector(data):
    pass

def h_vector(data):
    pass

def g_matrix(data):
    pass

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
def p_matrix(data):
    pmatrix = np.matrix(np.zeros((len(data), len(data))))
    for i in list(range(len(data))):
        for j in list(range(len(data))):
            kernel = linear_kernel(data[i][:2], data[j][:2])
            pmatrix[i, j] = data[i][2] * data[j][2] * kernel
    return pmatrix

def ind_function(supportVectors, x, y, support_vectors):
    indicator = 0
    for i in range(len(support_vectors)):
        alpha, d = support_vectors[i]
        indicator += alpha * y[2] * linear_kernel((x,y), d[:2])
    return indicator

def getAlpha(P, q, G, h):
    r = qp(matrix(P), matrix(q), matrix(G), matrix(h))
    alpha = list(r['x'])
    return alpha

def get_support_vectors(datapoints):
    #support_vectors = []
    #for i in list(range(len(data))):
    #    if abs(alphas[i]) > epsilon:
    #        support_vectors.append((alphas[i], data[i]))

    n = len(datapoints)
    alpha = getAlpha(p_matrix(datapoints), q(n), G(n), h(n))
    return removeZ(data,alpha)

def removeZ(datapoints,alpha):
    result = []
    for v1,v2 in zip(alpha, datapoints):
        if abs(v1) > epsilon:
            result.append((v1,v2))
    return result


def classify(x, support_vectors, kernel_function, **keys):
    point = ind_function(x, support_vectors, kernel_function, **keys)
    if point > 0.0:
        return 1.0
    return -1.0
    pass

def removeZ(datapoints,alpha):
    result = []
    for v1,v2 in zip(alpha, datapoints):
        if abs(v1) > epsilon:
            result.append((v1,v2))
    return result

def svm(data):

    datapoints,classA, classB = get_data()
    kernel_function = linear_kernel
    slack = False
    C = 100
    pylab.figure()
    pylab.title("Linear")
    pylab.plot([p[0] for p in classA],
               [p[1] for p in classA],
               'bo')
    pylab.plot([p[0] for p in classB],
               [p[1] for p in classB],
               'ro')

    t = get_support_vectors(datapoints)

    # Plot the decision boundaries.
    xr = numpy.arange(-4, 4, 0.05)
    yr = numpy.arange(-4, 4, 0.05)
    grid = matrix([[ind_function(t, x, y) for y in yr] for x in xr])
    pylab.contour(xr, yr, grid, (-1.0, 0.0, 1.0), colors=('red', 'black', 'blue'), linewidths=(1, 3, 1))


def main():
    svm(data)

if __name__ == '__main__':
    main()
