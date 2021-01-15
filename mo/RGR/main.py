# -*- coding: utf-8 -*-
import math
import numpy as np
import matplotlib.pylab as pl

count = 0

def incCount():
    global count
    count = count + 1

def calcQuasiX(x0, A, grad, lmb):
    '''
        x0 - gradient*lambda
    '''
    lmbA = np.dot(lmb, A)
    lmbAgrad = np.dot(lmbA, grad)
    calc = x0 - lmbAgrad
    return calc

def svennQuasi(x0, grad, lmb, delta, A):
    """
        One-dimensional Svenn search
    """
    f0 = fun(calcQuasiX(x0, A, grad, lmb))
    if f0 < fun(calcQuasiX(x0, A, grad, lmb + delta)):
        delta = -delta
    x1 = lmb + delta
    f1 = fun(calcQuasiX(x0, A, grad, x1))
    while f1 < f0:
        delta *= 2
        lmb = x1
        x1 = lmb + delta
        f0 = f1
        f1 = fun(calcQuasiX(x0, A, grad, x1))
    a = lmb + delta / 2
    b = lmb - delta / 2
    f0 = fun(calcQuasiX(x0, A, grad, lmb))
    f1 = fun(calcQuasiX(x0, A, grad, b))

    if f0 < f1:
        if a < b:
            return [a, b]
        else:
            return [b, a]
    elif f1 < f0:
        if lmb < x1:
            return [lmb, x1]
        else:
            return [x1, lmb]
    else:
        if lmb < b:
            return [lmb, b]
        else:
            return [b, lmb]

def dscQuasi(x0, grad, lmb, delta, A):
    svenn_res = svennQuasi(x0, grad, lmb, delta, A)
    x1 = svenn_res[0]
    x3 = svenn_res[1]
    x2 = (x1 + x3) / 2
    f1 = fun(calcQuasiX(x0, A, grad, x1))
    f2 = fun(calcQuasiX(x0, A, grad, x2))
    f3 = fun(calcQuasiX(x0, A, grad, x3))
    xApprox = x2 + ((x3 - x2) * (f1 - f3)) / (2 * (f1 - 2 * f2 + f3))
    return [x1, x2, x3, xApprox]

def dscPowellQuasi(x0, grad, eps, lmb, delta, A):
    dsc_res = dscQuasi(x0, grad, lmb, delta, A)
    a = dsc_res[0]
    xmin = dsc_res[1]
    b = dsc_res[2]
    xApprox = dsc_res[3]

    while abs(xmin - xApprox) >= eps or abs(
            fun(calcQuasiX(x0, A, grad, xmin)) - fun(calcQuasiX(x0, A, grad, xApprox))) >= eps:
        if xApprox < xmin:
            b = xmin
        else:
            a = xmin
        xmin = xApprox
        funcRes = [fun(calcQuasiX(x0, A, grad, a)), fun(calcQuasiX(x0, A, grad, xmin)), fun(calcQuasiX(x0, A, grad, b))]
        a1 = (funcRes[1] - funcRes[0]) / (xmin - a)
        a2 = ((funcRes[2] - funcRes[0]) / (b - a) - a1) / (b - xmin)
        xApprox = (a + xmin) / 2 - a1 / (2 * a2)
    return xmin

def goldQuasi(a, b, eps, x0, grad, A):
    """
        One-dimensional gold search
    """
    l = sub(b, a)
    x1 = add(a, mults(l, 0.382))
    x2 = add(a, mults(l, 0.618))
    while l > eps:
        if fun(calcQuasiX(x0, A, grad, x1)) < fun(calcQuasiX(x0, A, grad, x2)):
            b = x2
            x2 = x1
            l = sub(b, a)
            x1 = add(a, mults(l, 0.382))
            print "gold a: " + str(a)
            print "gold b: " + str(b)
        else:
            a = x1
            x1 = x2
            l = sub(b, a)
            x2 = add(a, mults(l, 0.618))
            print "gold a: " + str(a)
            print "gold b: " + str(b)

    return [a, b]

def calcLambdaQuasi(x0, grad, eps, lmb, A):
    line = svennQuasi(x0, grad, lmb, 0.1, A)
    line = goldQuasi(line[0], line[1], eps, x0, grad, A)
    lmb = (line[0] + line[1]) / 2
    return lmb

def plot(points, col): # для побудови графіка іункції
    n = 256
    x = np.linspace(-12, 12, n)
    y = np.linspace(-12, 12, n)
    X, Y = np.meshgrid(x, y)
    xs = []
    ys = []
    pl.contourf(X, Y, fun([X, Y]), 8, alpha=.75, cmap='jet')
    pl.contour(X, Y, fun([X, Y]), 8, colors='black')
    for i in range(len(points)):
        xs.append(points[i][0])
        ys.append(points[i][1])
    pl.plot(xs, ys, marker='o', linestyle='--', color=str(col), label='Square')
    pl.show()

def add(x, y):
    res = []
    for i in xrange(len(x)):
        res.append(x[i] + y[i])
    return res

def sub(x, y):
    res = []
    for i in xrange(len(x)):
        res.append(x[i] - y[i])
    return res

def mults(x, n):
    res = []
    for i in xrange(len(x)):
        res.append(x[i] * n)
    return res

def derivative(x, n):
    h = []
    for i in xrange(len(x)):
        if i == n:
            h.append(1e-5)
        else:
            h.append(0)
    return (fun([x[0] + h[0], x[1] + h[1]]) - fun([x[0] - h[0], x[1] - h[1]])) / (2 * h[n])

def derivative2(x, a, b):
    ai = []
    aj = []
    for i in xrange(len(x)):
        if i == a:
            ai.append(0.001)
        else:
            ai.append(0)
    for j in xrange(len(x)):
        if j == b:
            aj.append(0.001)
        else:
            aj.append(0)
    return (fun(add(x, add(ai, aj))) - fun(add(x, ai)) - fun(add(x, aj)) + fun(x)) / (ai[a] ** 2)

def gradient(x):
    grad = []
    for i in xrange(len(x)):
        grad.append(derivative(x, i))
    return grad

def norm(s1):
    normas = 0
    for i in xrange(len(s1)):
        normas += s1[i] ** 2
    return math.sqrt(normas)

def hesse(x):
    h = []
    for i in xrange(len(x)):
        for j in xrange(len(x)):
            h.append(derivative2(x, i, j))
    return h

def fun(x): # задаємо функцію
    incCount()
    return 2*x[0]**2 + x[0]*x[1] + x[1]**2
    # return (x[0]-1)**2 + 100*(x[0]**2 - x[1])**2

def dfp(x0, eps1, eps2):
    restart = 0
    iteration = 0
    xs = []
    xs.append(x0)
    lmb = 0.01
    A = np.eye(len(x0))

    print "Початкова точка"
    print x0
    print ""

    while True:
        grad = gradient(x0)
        if iteration > 0:
            if norm(grad) < eps1:
                print "РЕЗУЛЬТАТ РОБОТИ ПРОГРАМИ"
                print "Лічильник функцій"
                print count
                print "Кількість ітерацій"
                print iteration
                print "Перезапуск"
                print restart
                plot(xs, 'red')
                break
        lmb = dscPowellQuasi(x0, grad, eps2, lmb, 0.01, A)
        if lmb < 0:
            print "Перезапуск"
            A = np.eye(len(x0))
            restart = restart + 1

        x1 = calcQuasiX(x0, A, grad, lmb)
        deltag = np.array(sub(gradient(x1), gradient(x0)))[np.newaxis]
        deltax = sub(x1, x0)
        deltax = np.array(deltax)[np.newaxis]
        deltaxT = np.array(deltax).T
        deltagT = np.array(deltag).T
        deltax, deltaxT = deltaxT, deltax
        deltag, deltagT = deltagT, deltag
        first = np.dot(deltax, deltaxT)
        second = np.dot(deltaxT, deltag)
        resOne = first / second
        first = np.dot(A, deltag)
        second = np.dot(first, deltagT)
        third = np.dot(second, A)
        temp = third
        first = np.dot(deltagT, A)
        second = np.dot(first, deltag)
        resTwo = temp / second
        A = A + resOne - resTwo
        x0 = x1
        xs.append(x0)

        print "Точка"
        print x1
        print "Лічильник функцій"
        print count
        print "Лямбда"
        print lmb
        print "INV A"
        print np.linalg.inv(A)
        print "-------------------------------"
        iteration += 1

    print ""
    print "Мінімум"
    print x1

def main(): # початкова точка і крок по X та Y
    dfp([5, 5], 0.0001, 0.0001)

if __name__ == '__main__':
    main()


