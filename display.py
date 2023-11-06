import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

xbrut = open("cmake-build-debug/data/x.txt", "r")
ybrut = open("cmake-build-debug/data/y.txt", "r")

# split with " " between values and "\n" between lines

def format(a):
	a = a.read().split("\n")[1:-1]
	for i in range(len(a)):
		a[i] = a[i].split(" ")[1:-1]
	return np.array(a, dtype=float)

x = format(xbrut)
print(x)

y = format(ybrut)

fig = plt.figure()
ax = plt.axes(xlim=(0, 2000), ylim=(0, 2000))


lines = [ax.plot([], [], lw=2)[0] for _ in range(len(x[0]))]

def init():
	for j in range(len(x[0])):
		lines[j].set_data([], [])
	return lines


def animate(i):
	for j in range(len(x[0])):
		lines[j].set_data(x[:i, j], y[:i, j])
	return lines



anim = animation.FuncAnimation(fig, animate, init_func=init, frames=2000, interval=10, blit=True)

plt.show()



