"""
=================
An animated image
=================

This example demonstrates how to animate an image.
"""
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial

fig = plt.figure()
ax = fig.add_subplot(111)

arduino = serial.Serial("COM6",timeout=1, baudrate=9600)

def f():
    distance = str(arduino.readline())
    distance = distance[2:-5]
    try:
        distance = float(distance)
    except ValueError:
        distance = 0.0
    return np.array([[distance, 1000], [1000, 1000]])

im = ax.imshow(f(), cmap='gray', vmin=0, vmax=255, animated=True)
cbar = ax.figure.colorbar(im, ax=ax)
cbar.ax.set_ylabel("Distance Scale", rotation=-90, va="bottom")


def updatefig(*args):
    global distance
    im.set_array(f())
    return im,

ani = animation.FuncAnimation(fig, updatefig, interval=2, blit=True)
plt.show()