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
# Dictionnary of the parameters of the simulation

rec = np.genfromtxt('data/rec.txt', delimiter=' ', dtype=float) 
# Numpy array containing the x and y coordinates of the receptors (fixed)

abs = np.genfromtxt('data/nbr_absorption.txt', delimiter=' ', dtype=int) 
# Numpy array containing the number of chemoattractants absorbed by each receptor (columns) at each time step (lines)


# ANIMATION PARAMETERS

frames = []

width = Lx = int(param['Lx'])
height = Ly = int(param['Ly'])
Rcell = param['Rcell']
Rrec = int(param['Rrec'])

naff = len(m.open_txt_line(0,"data/x.txt")) 			    # Number of chemoattractants to plot
nt = int(param['time_max']/param['dt'])				        # Number of frames to plot
img = Image.new('RGB', (width, height), color='black')		# Create a default image

# ANIMATION GENERATION


print(' ')
def generate_frame(time):
    print('Generation of frame ', time, ' / ', nt, end='\r')

    # create a new image
    img = Image.new('RGB', (width, height), color='black')
    draw = ImageDraw.Draw(img)

    # draw the cell
    draw.ellipse((Lx / 2 - Rcell, Ly / 2 - Rcell, Lx / 2 + Rcell, Ly / 2 + Rcell), fill=(1, 30, 30))

    # load the chemoattractants coordinates for the current time
    x = m.open_txt_line(time, 'data/x.txt')
    y = m.open_txt_line(time, 'data/y.txt')

    # draw the chemoattractants
    for chemo in range(len(x)):
        # if np.isnan(x[part]) == False:
        xi = x[chemo]
        yi = y[chemo]
        
        if 0 <= xi < width and 0 <= yi < height:
            r, g, b = img.getpixel((xi, yi))
            img.putpixel((xi, yi), (0, g + 40, 200))

    # Vector A : direction of the maximum of chemoattractants
    A = np.zeros(2)

    # draw the receptors
    for receptor in range(len(rec)):
        xi, yi = rec[receptor, 0], rec[receptor, 1]
        
        if abs[time, receptor] <= 5:
            ip = abs[time, receptor]
            r = int(40 + (200-40)*(ip)/5)
            g = int(150 + (40-150)*(ip)/5)
            b = int(150 + (40-150)*(ip)/5)
        else:
            r,g,b = 255,0,0
        
        draw.ellipse((xi - Rrec, yi - Rrec, xi + Rrec, yi + Rrec), fill=(r,g,b))
        
        # add the contribution of this receptor to the vector A, weighted by the number of chemoattractants absorbed
        A += abs[time, receptor]*np.array([xi - Lx/2, yi - Ly/2])

    A = A / np.sqrt(np.sum(A**2))
    A = A * (Rcell/2)
    a = Rcell/20
    th_A = np.arctan2(A[1], A[0])
    
    # draw the arrow of where the maximum of chemoattractants is, based on the receptors
    draw.line((Lx/2, Ly/2, Lx/2 + A[0], Ly/2 + A[1]), fill=(40,150,150), width=3)
    draw.line((Lx/2 + A[0], Ly/2 + A[1],
               Lx/2 + A[0] + a*(- np.cos(th_A) + np.sin(th_A)),
               Ly/2 + A[1] + a*(- np.cos(th_A) - np.sin(th_A))), fill=(40,150,150), width=3)
    draw.line((Lx/2 + A[0], Ly/2 + A[1],
               Lx/2 + A[0] + a*(- np.cos(th_A) - np.sin(th_A)),
               Ly/2 + A[1] + a*(+ np.cos(th_A) - np.sin(th_A))), fill=(40,150,150), width=3)
    
    return np.array(img)

# generate frames concurrently
with ThreadPoolExecutor(6) as executor:
    frames = list(executor.map(generate_frame, range(2,nt)))

with imageio.get_writer('Static_cell_catching_chemo.mp4', format='FFMPEG', fps=20) as writer:
    for frame in frames:
        writer.append_data(frame)

t1 = time.time()

print(' ')
print('Animation generated in ', t1-t0, ' seconds')