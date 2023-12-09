import methods as m
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


Matrix = np.genfromtxt('data/save_abs_speed.txt', delimiter=' ', dtype=float)

p = Matrix[:,0]
v = Matrix[:,1]
D = Matrix[:,2]

# p = p[D==100]
# v = v[D==100]

p = p[v>=10]
v = v[v>=10]

# p = p[v<1]
# v = v[v<1]

plt.figure(1)
plt.plot(v, p, 'o')
plt.xlabel('Speed of the cell')
plt.ylabel('Average number of chemoattractants absorbed by each receptor')
plt.title('Average number of chemoattractants absorbed by each receptor as a function of the speed of the cell')
plt.yscale('log')
plt.xscale('log')


# add a linear regression
from scipy import stats


# # select only the point for which v > 10
# p = p[v>=10]
# v = v[v>=10]

slope, intercept, r_value, p_value, std_err = stats.linregress(np.log(v), np.log(p))
print('slope = ', slope)
print('intercept = ', intercept)
print('r_value = ', r_value)
print('p_value = ', p_value)
print('std_err = ', std_err)

plt.plot(v, np.exp(intercept)*v**slope, 'k', label='slope = '+ str(np.round(slope,2)))
plt.legend()


plt.show()
