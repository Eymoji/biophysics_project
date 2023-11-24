C++ SIMULATION
- main.cpp : Main C++ file where all the simulation process is done
- chemo.cpp, chemo.h : Class of chemoattractants used in the simulation
- receptor.cpp, receptor.h : Class of receptors used in the simulation
- functions.cpp, functions.h : Tool functions

DATA
- x.txt : stocks the x coordinate of each chemoattractant (a column) at each time t (a line)
- y.txt : stocks the y coordinate of each chemoattractant (a column) at each time t (a line)
- rec.txt : stocks the coordinates (x, y) of each receptor (a line)
- param.csv : First line, name of parameters, second line, their values.
- nbr_absorption.txt : stocks the number of collisions with each receptor (a column) at each time t (a line)

PYTHON OUTPUTS
- display.py : Reads the datafiles and generates a mp4 video