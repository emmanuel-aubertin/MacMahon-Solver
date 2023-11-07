# This script is here to create graph from compute time of the solver

import matplotlib.pyplot as plt
import numpy as np
import json

# Load the data from duration.json
with open("duration.json", "r") as file:
    data = json.load(file)

# Function to calculate basic stats
def basic_statistics(arr):
    mean = np.mean(arr)
    median = np.median(arr)
    std_dev = np.std(arr)
    return {
        'mean': mean,
        'median': median,
        'std_dev': std_dev,
    }

plt.figure(figsize=(10, 6))
print("# [Name] solver")

for grid, values in data.items():
    plt.plot(values, label=grid)
    stats = basic_statistics(values)
    print(f"## Statistics for {grid}:")
    for key, value in stats.items():
        print(f"- {key}: {value:.2f}")
    print("\n")

# Graph label definition
plt.xlabel('Iterations')
plt.ylabel('Time (ms)')
plt.legend()
plt.title('Solving process time for sequential')
plt.grid(True)
plt.tight_layout()
plt.show()
