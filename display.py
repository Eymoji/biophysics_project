import numpy as np
import methods as m
from PIL import Image, ImageDraw
import imageio
from concurrent.futures import ThreadPoolExecutor


# DATA LOADING

param = m.format_dict("data/param.csv")
x = m.format("data/x.txt")
y = m.format("data/y.txt")
rec = m.format("data/rec.txt")
abs = m.format("data/nbr_absorption.txt")

# ANIMATION PARAMETERS

frames = []

width = Lx = int(param['Lx'])
height = Ly = int(param['Ly'])
Rcell = param['Rcell']
Rrec = param['Rrec']

naff = len(x[0]) 											# Number of chemoattractants to plot
nt = int(param['time_max']/param['dt']) 					# Number of frames to plot
img = Image.new('RGB', (width, height), color='black')		# Create a default image

# ANIMATION GENERATION

def generate_frame(time):
	print('Generation of frame ', time, ' / ', nt, end='\r')

	img = Image.new('RGB', (width, height), color='black')
	draw = ImageDraw.Draw(img)

	# draw the cell
	draw.ellipse((Lx / 2 - Rcell, Ly / 2 - Rcell, Lx / 2 + Rcell, Ly / 2 + Rcell), fill=(0, 30, 30))

	# draw the chemoattractants
	for part in range(naff):
		xi = int(x[time, part])
		yi = int(y[time, part])
		if 0 <= xi < width and 0 <= yi < height:
			r, g, b = img.getpixel((xi, yi))
			img.putpixel((xi, yi), (r + 40, g + 40, 200))

	# draw the receptors
	for i in range(len(rec)):
		xi, yi = rec[i, 0], rec[i, 1]
		if 0 <= xi < width and 0 <= yi < height:
			draw.ellipse((xi - Rrec, yi - Rrec, xi + Rrec, yi + Rrec), fill=(40, 150, 150))
			for ip in range(10):
				if abs[time-ip, i] > 0.5:
					r = int(40 + (200-40)*(10-ip)/10)
					g = int(150 + (40-150)*(10-ip)/10)
					b = int(150 + (40-150)*(10-ip)/10)
					draw.ellipse((xi - Rrec, yi - Rrec, xi + Rrec, yi + Rrec), fill=(r,g,b))
					break
	return np.array(img)

# generate frames concurrently
with ThreadPoolExecutor() as executor:
    frames = list(executor.map(generate_frame, range(2,nt)))

with imageio.get_writer('Static_cell_catching_chemo.mp4', format='FFMPEG', fps=20) as writer:
    for frame in frames:
        writer.append_data(frame)
