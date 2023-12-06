import numpy as np
import methods as m
import csv
from PIL import Image, ImageDraw
import imageio
from concurrent.futures import ThreadPoolExecutor
import time

t0 = time.time()

# DATA LOADING

param = m.format_dict("data/param.csv")

# x = np.genfromtxt('data/x.txt', delimiter=' ', dtype=float, missing_values='NaN', filling_values=np.nan)
# y = np.genfromtxt('data/y.txt', delimiter=' ', dtype=float, missing_values='NaN', filling_values=np.nan)
rec = np.genfromtxt('data/rec.txt', delimiter=' ', dtype=float)
abs = np.genfromtxt('data/nbr_absorption.txt', delimiter=' ', dtype=int)

# how to read only the first 10 lines of a txt file with some missing values
# x = np.genfromtxt('data/x.txt', delimiter=' ', dtype=float, missing_values='NaN', filling_values=np.nan, max_rows=10)
#then read the next 10 lines
# x = np.genfromtxt('data/x.txt', delimiter=' ', dtype=float, missing_values='NaN', filling_values=np.nan, skip_header=10, max_rows=10)


# ANIMATION PARAMETERS

frames = []

width = Lx = int(param['Lx'])
height = Ly = int(param['Ly'])
Rcell = param['Rcell']
Rrec = param['Rrec']

naff = len(m.open_txt_line(0,"data/x.txt")) 			    # Number of chemoattractants to plot
nt = int(param['time_max']/param['dt']) 					# Number of frames to plot
img = Image.new('RGB', (width, height), color='black')		# Create a default image

# ANIMATION GENERATION

print(' ')
def generate_frame(time):
    print('Generation of frame ', time, ' / ', nt, end='\r')

    img = Image.new('RGB', (width, height), color='black')
    draw = ImageDraw.Draw(img)

    # draw the cell
    draw.ellipse((Lx / 2 - Rcell, Ly / 2 - Rcell, Lx / 2 + Rcell, Ly / 2 + Rcell), fill=(0, 30, 30))

    x = m.open_txt_line(time, 'data/x.txt')
    y = m.open_txt_line(time, 'data/y.txt')
    # draw the chemoattractants
    for part in range(len(x)):
        if np.isnan(x[part]) == False:
            xi = x[part]
            yi = y[part]
            if 0 <= xi < width and 0 <= yi < height:
                r, g, b = img.getpixel((xi, yi))
                img.putpixel((xi, yi), (r + 40, g + 40, 200))

    # draw the receptors
    for i in range(len(rec)):
        xi, yi = rec[i, 0], rec[i, 1]
        if 0 <= xi < width and 0 <= yi < height:
            
            if abs[time, i] <= 5:
                ip = abs[time, i]
                r = int(40 + (200-40)*(ip)/5)
                g = int(150 + (40-150)*(ip)/5)
                b = int(150 + (40-150)*(ip)/5)
                draw.ellipse((xi - Rrec, yi - Rrec, xi + Rrec, yi + Rrec), fill=(r,g,b))
            else:
                draw.ellipse((xi - Rrec, yi - Rrec, xi + Rrec, yi + Rrec), fill=(255, 0, 0))
            
    return np.array(img)

# generate frames concurrently
with ThreadPoolExecutor() as executor:
    frames = list(executor.map(generate_frame, range(2,nt)))

with imageio.get_writer('Static_cell_catching_chemo.mp4', format='FFMPEG', fps=20) as writer:
    for frame in frames:
        writer.append_data(frame)

t1 = time.time()

print(' ')
print('Animation generated in ', t1-t0, ' seconds')