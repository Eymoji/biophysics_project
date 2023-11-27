import methods as m
import numpy as np


param = m.format_dict("data/param.csv")
# Dictionnary of the parameters of the simulation
Lx = int(param['Lx']) # x and y dimensions of the simulation
Ly = int(param['Ly'])
Rcell = param['Rcell'] # Radius of the cell
Rrec = param['Rrec'] # Radius of the receptors
time_max = param['time_max'] # Total time of the simulation
dt = param['dt'] # Time step of the simulation


x = m.format("data/x.txt")
y = m.format("data/y.txt")
# Numpy Matrixes containing the x and y coordinates of the chemoattractants, 
# with lines corresponding to time and columns to the different chemoattractants

rec = m.format("data/rec.txt") 
# Numpy Matrix containing the x and y coordinates of the receptors (fixed)

abs = m.format("data/nbr_absorption.txt") 
# Numpy Matrix containing the number of chemoattractants absorbed by each receptor (columns) at each time step (lines)
