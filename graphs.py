import methods as m
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from methods import open_txt_line_with_nan
from scipy.optimize import curve_fit

# color = {1: 'lightblue', 2: 'blue', 3: 'darkblue', 4: 'black'}

# def func(x, a, b, c):
# 	return a * np.log((x + b)) + 1 - a * np.log(b)

# param = m.format_dict("data/param.csv") # Dictionnary of the parameters of the simulation

# receptors_coordinate = np.genfromtxt('data/rec.txt', delimiter=' ', dtype=float)
# # Numpy Matrix containing the x and y coordinates of the receptors (fixed)

# number_of_absorption = np.genfromtxt('data/nbr_absorption.txt', delimiter=' ', dtype=int)
# # Numpy Matrix containing the number of chemoattractants absorbed by each receptor (columns) at each time step (lines)

# Lx = int(param['Lx']) # x and y dimensions of the simulation
# Ly = int(param['Ly'])
# Radius_cell = param['Rcell'] # Radius of the cell
# Radius_receptor = param['Rrec'] # Radius of the receptors
# time_max = param['time_max'] # Total time of the simulation
# dt = param['dt'] # Time step of the simulation

# nt = int(time_max/dt) # Number of time steps
# print('Number of time steps: ', nt)

# x = open_txt_line_with_nan(1, 'data/x.txt')

# n = len(x)
# nt = int(time_max/dt)

# ###############################

# Survived = np.zeros(n)


# for time_step in range(1, nt):
    
# 	x = open_txt_line_with_nan(time_step, 'data/x.txt')
 
# 	for chemo in range(n):
# 		if x[chemo] == -1 and Survived[chemo] == 0:
# 			Survived[chemo] = 1

# 	print(time_step, ' / ', nt, end='\r')
 
# 	if time_step in [nt//4, nt//2, 3*nt//4, nt-1]:
		

# 		x0 = open_txt_line_with_nan(1, 'data/x.txt')
# 		y0 = open_txt_line_with_nan(1, 'data/y.txt')

# 		initial_distance = np.sqrt((x0 - Lx/2)**2 + (y0 - Ly/2)**2) / Radius_cell

# 		###############################

# 		# plot the histogram of probability of being captured as a function of the initial distance of the chemoattractants to the cell
# 		dist_bins = np.arange(1, (Lx/np.sqrt(2))/Radius_cell + 1, 0.001)
# 		bin_length = (dist_bins[-1] - dist_bins[0]) / len(dist_bins)
# 		print('bin_length: ', bin_length)

# 		np.savetxt('data/Survived_{t}'.format(t=time_step) + '.txt', Survived)
# 		np.savetxt('data/initial_distance_{t}'.format(t=time_step) + '.txt', initial_distance)

# 		################################

# 		# Survived = np.loadtxt('data/Survived_{t}'.format(t=time_step) + '.txt')
# 		# initial_distance = np.loadtxt('data/initial_distance_{t}'.format(t=time_step) + '.txt')

# 		nb_capture = np.zeros(len(dist_bins))
# 		nb_non_capture = np.zeros(len(dist_bins))
# 		for chemo in range(n):    
# 			if initial_distance[chemo] > dist_bins[-1]:
# 				continue

# 			dist_bin = int((initial_distance[chemo] - dist_bins[0]) / bin_length)

# 			if Survived[chemo] == 1:
# 				nb_capture[dist_bin] += 1
# 			else:
# 				nb_non_capture[dist_bin] += 1


# 		nb_tot = nb_capture + nb_non_capture

# 		dist_bins = dist_bins[nb_tot != 0]
# 		nb_capture = nb_capture[nb_tot != 0]
# 		nb_tot = nb_tot[nb_tot != 0]

# 		prob_capture = nb_capture / nb_tot

# 		np.savetxt('data/dist_bins.txt_{t}'.format(t=time_step) + '.txt', dist_bins)
# 		np.savetxt('data/prob_capture_{t}'.format(t=time_step) + '.txt', prob_capture)

# 		###############################

# 		# dist_bins = np.loadtxt('data/dist_bins_{t}'.format(t=time_step) + '.txt')
# 		# prob_capture = np.loadtxt('data/prob_capture_{t}'.format(t=time_step) + '.txt')

# 		dist_bins = dist_bins[prob_capture>0] - 0.999
# 		prob_capture = prob_capture[prob_capture>0]

# 		prob_capture = prob_capture[dist_bins<0.6]
# 		dist_bins = dist_bins[dist_bins<0.6]


# 		plt.figure(1)
# 		plt.scatter(dist_bins, prob_capture, s=3, c=color[(4 * time_step) // nt], label='Simulation', alpha=0.5, marker='+')

# 		# p0 = [1, 1, 1]

# 		# popt, pcov = curve_fit(func, dist_bins, prob_capture, p0=p0)
# 		# a ,b, c = popt

# 		# print('b = ', b)

# 		# prob_pred = func(dist_bins, a, b, c)

# 		# plt.plot(dist_bins, prob_pred, label='$P(r) = \\alpha / (r/R_{cell}+\\alpha)$ fit : $\\alpha$ = ' + str(np.round(b,2)), color='darkblue', linewidth=2)

# plt.legend()
# plt.xlabel('Distance to the cell $r/R_{cell}$')
# plt.ylabel('Probability of being captured $P$')
# # plt.xscale('log')
# # plt.yscale('log')

# plt.show()
# plt.savefig('prob_capture.png', dpi=300)






def func(x, a, b):
	return a * np.log((x + b)) + 1 - a * np.log(b)

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

nt = int(time_max/dt) # Number of time steps
print('Number of time steps: ', nt)

x = open_txt_line_with_nan(1, 'data/x.txt')

n = len(x)
nt = int(time_max/dt)

###############################
color = {nt//4: 'crimson', nt//2: 'fuchsia', 3*nt//4: 'darkviolet', nt-1: 'darkblue'}
Survived = np.zeros(n)


for time_step in [nt//4, nt//2, 3*nt//4, nt-1]:

		Survived = np.loadtxt('data/Survived_{t}'.format(t=time_step) + '.txt')
		initial_distance = np.loadtxt('data/initial_distance_{t}'.format(t=time_step) + '.txt')

		###############################

		# plot the histogram of probability of being captured as a function of the initial distance of the chemoattractants to the cell
		dist_bins = np.arange(1, (Lx/np.sqrt(2))/Radius_cell + 1, 0.001)
		bin_length = (dist_bins[-1] - dist_bins[0]) / len(dist_bins)
		print('bin_length: ', bin_length)

		################################



		nb_capture = np.zeros(len(dist_bins))
		nb_non_capture = np.zeros(len(dist_bins))
		for chemo in range(n):    
			if initial_distance[chemo] > dist_bins[-1]:
				continue

			dist_bin = int((initial_distance[chemo] - dist_bins[0]) / bin_length)

			if Survived[chemo] == 1:
				nb_capture[dist_bin] += 1
			else:
				nb_non_capture[dist_bin] += 1


		nb_tot = nb_capture + nb_non_capture

		dist_bins = dist_bins[nb_tot != 0]
		nb_capture = nb_capture[nb_tot != 0]
		nb_tot = nb_tot[nb_tot != 0]

		prob_capture = nb_capture / nb_tot

		np.savetxt('data/dist_bins.txt_{t}'.format(t=time_step) + '.txt', dist_bins)
		np.savetxt('data/prob_capture_{t}'.format(t=time_step) + '.txt', prob_capture)

		###############################

		# dist_bins = np.loadtxt('data/dist_bins_{t}'.format(t=time_step) + '.txt')
		# prob_capture = np.loadtxt('data/prob_capture_{t}'.format(t=time_step) + '.txt')

		dist_bins = dist_bins[prob_capture>0] - 0.999
		prob_capture = prob_capture[prob_capture>0]

		prob_capture = prob_capture[dist_bins<0.6]
		dist_bins = dist_bins[dist_bins<0.6]


		plt.figure(1)
		plt.scatter(dist_bins, prob_capture, s=3, c=color[time_step], label='Simulation for $t = $' + str(int(time_step*dt)) + 's'
              , marker='+')

		p0 = [1, 1]

		popt, pcov = curve_fit(func, dist_bins, prob_capture, p0=p0)
		a ,b = popt

		print('a = ', a)
		print('b = ', b)

		prob_pred = func(dist_bins, a, b)

		plt.plot(dist_bins, prob_pred, label='Fit for $t = $' + str(int(time_step*dt)) + 's'
           , color=color[time_step], linewidth=2)

plt.legend()
plt.xlabel('Distance to the cell $r/R_{cell}$', fontsize=14)
plt.ylabel('Probability of being captured $P$', fontsize=14)
plt.xlim(0, 0.6)
plt.ylim(0, 1)
# plt.xscale('log')
# plt.yscale('log')
plt.savefig('prob_capture.png', dpi=300)