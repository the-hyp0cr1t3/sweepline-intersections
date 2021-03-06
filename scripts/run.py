#!/usr/bin/env python3

import sys
import os
import subprocess
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import seaborn as sns
from parse import search

sns.set_style("darkgrid")
sns.set_context("talk")
sns.set_palette("magma")

plt.rcParams["figure.edgecolor"] = "black"
plt.rcParams["axes.edgecolor"] = "black"
plt.rcParams["axes.linewidth"] = 2.50

OUT_PATH = "../build/run_output.txt"
EXEC_CMD = [ "../bin/app", "-nc", "-o" , OUT_PATH, "-i" ]

def getData(segmentPath):
    # Reads the line segments from segmentPath and intersection points from OUT_PATH
    segments = []
    try:
        with open(segmentPath, 'r') as f:
            lines = f.read().splitlines()
    except:
        with open(os.path.join("../data", segmentPath), 'r') as f:
            lines = f.read().splitlines()
    n = int(lines[0])
    for i in range(1, n+1):
        coords = [float(x) for x in lines[i].split()]
        segments.append([(coords[0], coords[1]), (coords[2], coords[3])])

    points = []
    with open(OUT_PATH, 'r') as f:
        lines = f.read().splitlines()
        m = int(lines[0])
        for i in range(1, m+1):
            points.append(tuple(search("({:f}, {:f})", lines[i])))

    return {"segments": segments, "points": points}

def plotData(data, ax=None, markerSize=50, monochrome = False):
    # Plots the segments and intersection points
    ax = plt.gca()
    for line in data["segments"]:
        # Plot the segments
        if len(data["segments"]) > 50:
            ax.plot([line[0][0], line[1][0]], [line[0][1], line[1][1]], zorder=1, color="blue")
        else:
            ax.scatter([line[0][0], line[1][0]], [line[0][1], line[1][1]], zorder=2, s=markerSize)
            ax.plot([line[0][0], line[1][0]], [line[0][1], line[1][1]], zorder=1)
    for point in data["points"]:
        # Plot the intersection points
        ax.scatter(point[0], point[1], color="orange", edgecolors="black", s=markerSize, zorder=3)
        scatterpoint = mlines.Line2D([], [], color="orange", marker="o", markersize=10, markeredgecolor="black", markeredgewidth=1.5, ls="")
        # ax.legend(handles=[scatterpoint], labels=[len(data["points"])], loc="upper right", prop={'size': 20, 'weight': 'bold'}, framealpha=1)
    plt.show()

def main():
    # Runs the algorithm to find intersection points on given input file and stores the intersection points in OUT_PATH
    subprocess.run(EXEC_CMD + [sys.argv[1]])

    print("Plotting graph...")
    plotData(getData(sys.argv[1]))

if __name__ == "__main__":
    main()