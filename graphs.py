import methods as m
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

param = m.format_dict("data/param.csv") # Dictionnary of the parameters of the simulation

receptors_coordinate = np.genfromtxt('data/rec.txt', delimiter=' ', dtype=float)
# Numpy Matrix containing the x and y coordinates of the receptors (fixed)

number_of_absorption = np.genfromtxt('data/nbr_absorption.txt', delimiter=' ', dtype=int)
# Numpy Matrix containing the number of chemoattractants absorbed by each receptor (columns) at each time step (lines)

Lx = int(param['Lx']) # x and y dimensions of the simulation
Ly = int(param['Ly'])
Radius_cell = param['Rcell'] # Radius of the cell
Radius_receptor = param['Rrec'] # Radius of the receptors
time_max = param['time_max'] # Total time of the simulation
dt = param['dt'] # Time step of the simulation


# Plot the average number of chemoattractants absorbed by each receptor as a function of time

plt.figure(1)
plt.plot(np.arange(0, time_max+2*dt, dt), np.mean(number_of_absorption, axis = 1))
plt.xlabel('time')
plt.ylabel('Average number of chemoattractants absorbed by each receptor')
plt.title('Average number of chemoattractants absorbed by each receptor as a function of time')
plt.show()
plt.close()

# do the same but with the standard deviation

plt.figure(2)
plt.plot(np.arange(0, time_max+2*dt, dt), np.std(number_of_absorption, axis = 1))
plt.xlabel('time')
plt.ylabel('Standard deviation of the number of chemoattractants absorbed by each receptor')
plt.title('Standard deviation of the number of chemoattractants absorbed by each receptor as a function of time')
plt.show()
plt.close()

# plot rolling average of the number of chemoattractants absorbed by each receptor as a function of time

plt.figure(3)
plt.plot(np.arange(0, time_max+2*dt, dt), np.mean(number_of_absorption, axis = 1), label = 'Average')
plt.plot(np.arange(0, time_max+2*dt, dt), pd.Series(np.mean(number_of_absorption, axis = 1)).rolling(window=1000).mean(), label = 'Rolling average')
plt.xlabel('time')
plt.ylabel('Average number of chemoattractants absorbed by each receptor')
plt.title('Average number of chemoattractants absorbed by each receptor as a function of time')
plt.legend()
plt.show()
plt.close()

