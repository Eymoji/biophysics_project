import numpy as np
import csv
from PIL import Image, ImageDraw
import imageio


# DATA LOADING

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
abs = format(open("data/nbr_absorption.txt", "r"))


# ANIMATION PARAMETERS

width = height = int(param['L'])
frames = []

L = param['L']
Rcell = param['Rcell']
Rrec = param['Rrec']

naff = len(x[0]) 											# Number of chemoattractants to plot
nt = int(param['time_max']/param['dt']) 					# Number of frames to plot
img = Image.new('RGB', (width, height), color='black')		# Create a default image

# ANIMATION GENERATION

for time in range(nt):
	print('frame : ', time, '/', nt, end='\r')

	ans_img = img
	img = Image.new('RGB', (width, height), color='black')
	draw = ImageDraw.Draw(img)

	# draw the cell
	draw.ellipse((L/2-Rcell, L/2-Rcell, L/2+Rcell, L/2+Rcell), fill=(0, 30, 30))
 
	# draw the chemoattractants
	for part in range(naff):
		xi = int(x[time, part])
		yi = int(y[time, part])
		if xi < width and yi < height:
			r,g,b = img.getpixel((xi, yi))
			img.putpixel((xi, yi), (r+40, g+40, 200))

	# draw the receptors
	for i in range(len(rec)):
		xi = int(rec[i][0])
		yi = int(rec[i][1])
		if xi < width and yi < height:
			if time==0:
				draw.ellipse((xi-Rrec, yi-Rrec, xi+Rrec, yi+Rrec), fill=(40, 150, 150)) # green initially
			elif abs[time,i] > 0.5:
				draw.ellipse((xi-Rrec, yi-Rrec, xi+Rrec, yi+Rrec), fill=(200, 40, 40)) # red if activated
			else:
				r,g,b = ans_img.getpixel((xi, yi))
				draw.ellipse((xi-Rrec, yi-Rrec, xi+Rrec, yi+Rrec), fill=(max(40,r-4), min(150,g+4), min(150,g+4))) # gradually turn green again
				
	frames.append(np.array(img))

# generate gif

frames = frames + frames[::-1]
imageio.mimsave('movie.gif', frames, fps=20)
