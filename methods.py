import numpy as np
import csv
import linecache
import pandas as pd

def format(file_path):
    with open(file_path, "r") as file:
        data = np.array([line.split(" ") for line in file.read().split(" \n")[:-1]], dtype=float)
    return data

def format_dict(file_path):
	dict =  [row for row in csv.DictReader(open(file_path, newline=''))][0]
	return {key: float(value) for key, value in dict.items()}

def open_txt_line(i, name):
    line = linecache.getline(name, i).split()
    return np.array(line, dtype=int)

def open_txt(name):
    data = np.genfromtxt(name, delimiter=' ', dtype=int, missing_values='NaN', filling_values=np.nan)
    return data