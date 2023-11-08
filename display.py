import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv


# Open and read DataFiles

param = [row for row in csv.DictReader(open("data/param.csv", newline=''))][0]
param = {key: float(value) for key, value in param.items()}


def format(a):
	a = a.read().split(" \n")[:-1]
	for i in range(len(a)):
		a[i] = a[i].split(" ")
	return np.array(a, dtype=float)

x = format(open("data/x.txt", "r"))
y = format(open("data/y.txt", "r"))
rec = format(open("data/rec.txt", "r"))
print(rec)

# Plotting and animation

naff = len(x[0])//5 # Number of chemoattractants to plot

fig = plt.figure()
# ax = plt.axes(xlim=(param['L']/2-2*param['Rcell'], param['L']/2+2*param['Rcell']), 
#               ylim=(param['L']/2-2*param['Rcell'], param['L']/2+2*param['Rcell']))
ax = plt.axes(xlim=(0, param['L']),ylim=(0, param['L']))
ax.set_facecolor('black')



lines = [ax.plot([], [], lw=2, color='cyan')[0] for _ in range(naff)]


def init():
	for j in range(naff):
		lines[j].set_data([], [])
	circle = plt.Circle((param['L']/2, param['L']/2), param['Rcell'], fill=False, color='white')
	ax.add_patch(circle)
 
	for i in range(len(rec)):
		ax.add_patch(plt.Circle((rec[i][0], rec[i][1]), param['Rrec'], fill=True, color='red'))
	return lines


def animate(i):
	print('frame : ', i)
	for j in range(naff):
		lines[j].set_data(x[max(0,i-5):i, j], y[max(0,i-5):i, j])
	return lines



anim = animation.FuncAnimation(fig, animate, init_func=init, frames=2000, interval=5, blit=True)

plt.show()


