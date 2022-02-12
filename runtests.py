from matplotlib import pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import numpy as np

""" m = pd.read_csv("output.csv")

m.sort_values('iteration', inplace = True)

print(m.columns.tolist())
iteration = m['iteration']
x = m[' X']
y = m[' Y']
inside = m[' Inside']

fig = plt.figure()
ax1 = fig.add_subplot(111)
circle = plt.Circle((0, 0), 1, color='black', fill=False, alpha=0.8)

def animate(i):
    ax1.clear()
    ax1.set_xlim([0, 2])
    ax1.set_ylim([0,2])
    ax1.add_patch(circle)
    colors = inside[:i].tolist()
    ax1.scatter(x[:i],y[:i],c=colors,marker="o", cmap="bwr_r")

iteration = []
x = []
y= []
inside = []

for i in range(0,8):
    m = pd.read_csv("outputOMP" + str(i) + ".csv")

    m.sort_values(m.columns[0], inplace = True)

    iteration.append(m[m.columns[0]])
    x.append(m[m.columns[1]])
    y.append(m[m.columns[2]])
    inside.append(m[m.columns[3]])

fig = plt.figure()
ax1 = fig.add_subplot(111)
circle = plt.Circle((0, 0), 1, color='black', fill=False, alpha=0.8)

def animate(i):
    ax1.clear()
    ax1.set_xlim([0, 2])
    ax1.set_ylim([0,2])
    ax1.add_patch(circle)
    colors0 = np.array(inside[0].tolist()).astype(float)
    colors1 = np.array(inside[1].tolist()).astype(float)
    colors2 = np.array(inside[2].tolist()).astype(float)
    colors3 = np.array(inside[3].tolist()).astype(float)
    colors4 = np.array(inside[4].tolist()).astype(float)
    colors5 = np.array(inside[5].tolist()).astype(float)
    colors6 = np.array(inside[6].tolist()).astype(float)
    colors7 = np.array(inside[7].tolist()).astype(float)
    ax1.scatter(x[0][:i],y[0][:i],c=colors0[:i],marker="o", cmap="bwr_r", vmin=0, vmax=1, label='Thread 0')
    ax1.scatter(x[1][:i],y[1][:i],c=colors1[:i],marker="v", cmap="bwr_r", vmin=0, vmax=1,label='Thread 1')
    ax1.scatter(x[2][:i],y[2][:i],c=colors2[:i],marker="8", cmap="bwr_r", vmin=0, vmax=1,label='Thread 2')
    ax1.scatter(x[3][:i],y[3][:i],c=colors3[:i],marker="s", cmap="bwr_r", vmin=0, vmax=1,label='Thread 3')
    ax1.scatter(x[4][:i],y[4][:i],c=colors4[:i],marker="p", cmap="bwr_r", vmin=0, vmax=1,label='Thread 4')
    ax1.scatter(x[5][:i],y[5][:i],c=colors5[:i],marker="h", cmap="bwr_r", vmin=0, vmax=1,label='Thread 5')
    ax1.scatter(x[6][:i],y[6][:i],c=colors6[:i],marker="D", cmap="bwr_r", vmin=0, vmax=1,label='Thread 6')
    ax1.scatter(x[7][:i],y[7][:i],c=colors7[:i],marker="P", cmap="bwr_r", vmin=0, vmax=1,label='Thread 7')
    plt.legend(loc='upper left');


    

ani = animation.FuncAnimation(fig, animate, frames=100, interval = 400)
ani.save('pics/openmpparallel.gif', writer='imagemagick', fps=10)
#plt.show()"""

import subprocess
import time
import re
import csv

N = [10,100,1000,10000,100000,1000000,100000000,1000000000,10000000000, 100000000000]
cores = [2,4,6,8]

sequential = ["./Output/sequential"]
parallelopenmp = ["./Output/parallelopenmp"]
parallelmpi = ["mpirun", "-np", "Output/parallelmpi"]
header = ['N', 'pi_value', 'error_value', 'time']

"""
with open('seriale.csv', 'w', encoding='UTF8') as f:
        writer = csv.writer(f)
        writer.writerow(header)
        for n in N:
            print("running serial version with " + str(n) + " iterations")
            ts = time.time()
            output = subprocess.run(["./Output/sequential", str(n)], capture_output=True)
            te = time.time()
            elapsed_time = te - ts
            nums=re.findall(r'[\d]*[.][\d]+(?:[eE][-+]?[0-9]+)?', output.stdout.decode('utf-8'))
            print(nums)
            writer.writerow([n, nums[0], nums[1], elapsed_time])
            f.flush()

"""
for core in cores:
    with open('openmp' + str(core) + '.csv', 'w', encoding='UTF8') as f:
            writer = csv.writer(f)
            writer.writerow(header)
            for n in N:
                print("running openmp version with " + str(core) +" cores " + str(n) + " iterations")
                ts = time.time()
                output = subprocess.run(["./Output/parallelopenmp", str(n), str(core)], capture_output=True)
                te = time.time()
                elapsed_time = te - ts
                nums=re.findall(r'[\d]*[.][\d]+(?:[eE][-+]?[0-9]+)?', output.stdout.decode('utf-8'))
                writer.writerow([n, nums[0], nums[1], elapsed_time])
                f.flush()

for core in cores:
    with open('mpi' + str(core) + '.csv', 'w', encoding='UTF8') as f:
            writer = csv.writer(f)
            writer.writerow(header)
            for n in N:
                print("running mpi version with " + str(core) +" nodes "+ str(n) + " iterations")
                ts = time.time()
                output = subprocess.run(["mpirun", "-np", str(core), "Output/parallelmpi", str(n)], capture_output=True)
                te = time.time()
                elapsed_time = te - ts
                nums=re.findall(r'[\d]*[.][\d]+(?:[eE][-+]?[0-9]+)?', output.stdout.decode('utf-8'))
                writer.writerow([n, nums[0], nums[1], elapsed_time])
                f.flush()
