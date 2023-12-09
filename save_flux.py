import methods as m
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

param = m.format_dict("data/param.csv") # Dictionnary of the parameters of the simulation

# receptors_coordinate = np.genfromtxt('data/rec.txt', delimiter=' ', dtype=float)
# Numpy Matrix containing the x and y coordinates of the receptors (fixed)

number_of_absorption = np.genfromtxt('data/nbr_absorption.txt', delimiter=' ', dtype=int)
# Numpy Matrix containing the number of chemoattractants absorbed by each receptor (columns) at each time step (lines)

Lx = int(param['Lx']) # x and y dimensions of the simulation
Ly = int(param['Ly'])
Radius_cell = param['Rcell'] # Radius of the cell
Radius_receptor = param['Rrec'] # Radius of the receptors
time_max = param['time_max'] # Total time of the simulation
dt = param['dt'] # Time step of the simulation
speed_cell = param['speed_cell'] # Speed of the cell
D = param['D'] # Diffusion coefficient of the chemoattractants

# compute the average number of chemoattractants absorbed by all receptors at all time steps

average_number_of_absorption = np.mean(number_of_absorption)


with open('data/save_abs_speed.txt', 'a') as file:
    file.write(str(average_number_of_absorption) + ' ' + str(speed_cell) + ' ' + str(D) + ' ' + str(Radius_cell) +'\n')