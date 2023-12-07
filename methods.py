import numpy as np
import csv
import linecache
import pandas as pd

# Method to open a csv file and return a dictionary
def format_dict(file_path):
	dict =  [row for row in csv.DictReader(open(file_path, newline=''))][0]
	return {key: float(value) for key, value in dict.items()}

# Method to open a txt file and return a numpy array
def open_txt(name):
    data = np.genfromtxt(name, delimiter=' ', dtype=int, missing_values='NaN', filling_values=np.nan)
    return data

# Method to open a txt file and return a numpy array of the line i
def open_txt_line(i, name):
    line = linecache.getline(name, i).split()
    return np.array(line, dtype=int)