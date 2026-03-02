import numpy as np
import sympy as sp

def to_frac(x: np.ndarray) -> sp.Matrix:
    return sp.Matrix(x).applyfunc(sp.nsimplify)


# Stationary dist.
P = np.array([[0.5, 0.3, 0.2],
              [0.2, 0.5, 0.3],
              [0.1, 0.4, 0.5]])
P -= np.eye(P.shape[0])
P = P.T
P[-1] = 1
P = to_frac(P)

b = np.zeros(P.shape[1])
b[-1] = 1
b = sp.Matrix(b).applyfunc(sp.nsimplify)

print(P.LUsolve(b))


# Exit dist.
R = np.array([[0,   0.5, 0  ],
              [0.5, 0  , 0.5],
              [0  , 0.5, 0  ]])
R = np.eye(R.shape[0]) - R
R = to_frac(R)
print(R)

v = np.array([0, 0, 0.5])
v = to_frac(v)

print(R.LUsolve(v))


# Exit time.
R = np.array([[0,   0.5, 0  ],
              [0.5, 0  , 0.5],
              [0  , 0.5, 0  ]])
R = np.eye(R.shape[0]) - R
R = to_frac(R)

b = np.ones(R.shape[0])
b = to_frac(b)

print(R.LUsolve(b))
