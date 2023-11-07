import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv

# Open and read DataFiles

xbrut = open("data/x.txt", "r")
ybrut = open("data/y.txt", "r")

param = [row for row in csv.DictReader(open("data/param.csv", newline=''))][0]

def format(a):
	a = a.read().split("\n")[1:-1]
	for i in range(len(a)):
		a[i] = a[i].split(" ")[1:-1]
	return np.array(a, dtype=float)

x = format(xbrut)
y = format(ybrut)

fig = plt.figure()
ax = plt.axes(xlim=(0, 2000), ylim=(0, 2000))

naff = len(x[0])//10


lines = [ax.plot([], [], lw=2)[0] for _ in range(naff)]


def init():
	for j in range(naff):
		lines[j].set_data([], [])
	circle = plt.Circle((0, 0), float(param['Rcell']), fill=False, color='black')
	ax.add_patch(circle)
	return lines


def animate(i):
	for j in range(naff):
		lines[j].set_data(x[:i, j], y[:i, j])
	return lines



anim = animation.FuncAnimation(fig, animate, init_func=init, frames=2000, interval=1, blit=True)

plt.show()



