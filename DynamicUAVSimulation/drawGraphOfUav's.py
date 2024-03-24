# -*- coding: utf-8 -*-
"""
Created on Wed Mar 20 01:56:18 2024

@author: 97254
"""
import matplotlib.pyplot as plt

# Read simulation parameters from SimParams.ini
with open("SimParams.ini", "r") as params_file:
    params_data = params_file.readlines()

# Extract N_uav value from the parameters
N_uav = None
for line in params_data:
    if line.startswith("N_uav"):
        N_uav = int(line.split("=")[1])
        break

if N_uav is None:
    print("Error: N_uav not found in SimParams.ini")
    exit()

# Loop through each UAV file
for uav_num in range(1, N_uav + 1):
    # Read data from the text file for the current UAV
    with open(f"UAV{uav_num}.txt", "r") as file:
        data = file.readlines()

    # Extract x and y coordinates from the data
    x_values = []
    y_values = []
    for line in data:
        parts = line.strip().split()
        x_values.append(float(parts[1]))  # x coordinate
        y_values.append(float(parts[2]))  # y coordinate

    # Plot the graph with filled markers
    plt.figure(figsize=(8, 6))
    plt.plot(x_values, y_values, marker='o', linestyle='-', markerfacecolor='black')  # Modify markerfacecolor to your desired color
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title(f'Graph of X and Y Coordinates for UAV {uav_num}')
    plt.grid(True)
    plt.axis('equal')  # Set equal aspect ratio
    plt.show()
    