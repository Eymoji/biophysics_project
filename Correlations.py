import methods as m
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

param = m.format_dict("data/param.csv")

Lx = int(param['Lx']) # x and y dimensions of the simulation
Ly = int(param['Ly'])
Radius_cell = param['Rcell'] # Radius of the cell
Radius_receptor = param['Rrec'] # Radius of the receptors
time_max = param['time_max'] # Total time of the simulation
dt = param['dt'] # Time step of the simulation

receptors_coordinate = np.genfromtxt('data/rec.txt', delimiter=' ', dtype=float)
# Numpy Matrix containing the x and y coordinates of the receptors (fixed)

number_of_absorption = np.genfromtxt('data/nbr_absorption.txt', delimiter=' ', dtype=int)
# Numpy Matrix containing the number of chemoattractants absorbed by each receptor (columns) at each time step (lines)
number_iteration = np.shape(number_of_absorption)[0]
number_receptor = np.shape(number_of_absorption)[1]



Correlations_receptor = np.zeros((2*int(number_iteration/2)+1, number_receptor))

for i in range(-int(number_iteration/2), -1):

    for j in range(number_receptor):
        if ((number_of_absorption[-i:,j] == 1).any()) == True:
            Correlations_receptor[i+int(number_iteration/2),j] = np.mean(number_of_absorption[-i:,j]*number_of_absorption[:i,j], where=(number_of_absorption[-i:,j] == 1))
        else:
            Correlations_receptor[i+int(number_iteration/2),j] = Correlations_receptor[i+int(number_iteration/2)-1,j]
          
for i in range(1, int(number_iteration/2)):

    for j in range(number_receptor):

        if ((number_of_absorption[:-i,j] == 1).any()) == True:
            Correlations_receptor[i+int(number_iteration/2),j] = np.mean(number_of_absorption[:-i,j]*number_of_absorption[i:,j], where=(number_of_absorption[:-i,j] == 1))
        else:
            Correlations_receptor[i+int(number_iteration/2),j] = Correlations_receptor[i+int(number_iteration/2)-1,j]

for j in range(number_receptor):
    
    Correlations_receptor[int(number_iteration/2),j] = 1


plt.figure(figsize=(16,16))
Tau = np.arange(-int(number_iteration/2), int(number_iteration/2)+1)*dt
for j in range(number_receptor):
    plt.plot(Tau, Correlations_receptor[:,j])
plt.xlabel("tau (s)")
plt.ylabel("Correlation")
plt.show()

Averaged_Correlations = np.mean(Correlations_receptor, axis = 1)

plt.figure(figsize=(16,16))
plt.plot(Tau, Averaged_Correlations)
plt.xlabel("tau (s)")
plt.ylabel("Correlation")
plt.show()

for j in range(number_receptor):
    print(Correlations_receptor[int(number_iteration/2)-1,j])
print(Averaged_Correlations[int(number_iteration/2)-1])



def fit_correlations(tau, p_moy, tau_b):
    return p_moy**2 + p_moy*(1-p_moy)*np.exp(-abs(tau)/((1-p_moy)*tau_b))

popt, pcov = curve_fit(fit_correlations, Tau, Averaged_Correlations, p0 = np.array([0.5, 5]))
print(popt)


plt.figure(figsize=(16,16))
plt.plot(Tau, Averaged_Correlations)
plt.plot(Tau, fit_correlations(popt[0], popt[1], Tau))
plt.xlabel("tau (s)")
plt.ylabel("Correlation")
plt.show()
