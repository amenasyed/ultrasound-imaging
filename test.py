import matplotlib.pyplot as plt
import numpy as np
# import time
import serial

arduino = serial.Serial("COM6",timeout=1, baudrate=9600)

# create the figure
fig = plt.figure()
ax = fig.add_subplot(111)
im = ax.imshow(np.random.random((4, 4)))
plt.show(block=False)

# draw some data in loop
while True:
    distance = str(arduino.readline())
    distance = distance[2:-5]
    try:
        distance = float(distance)
    except ValueError:
        distance = 0.0
    print(distance)
    # replace the image contents
    im.set_array(np.array([[distance, 0], [0, 0]]))
    # redraw the figure
    fig.canvas.draw()
    fig.canvas.flush_events()