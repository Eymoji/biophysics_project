import numpy as np
import csv

def format(file_path):
    with open(file_path, "r") as file:
        data = np.array([line.split(" ") for line in file.read().split(" \n")[:-1]], dtype=float)
    return data

def format_dict(file_path):
	dict =  [row for row in csv.DictReader(open(file_path, newline=''))][0]
	return {key: float(value) for key, value in dict.items()}