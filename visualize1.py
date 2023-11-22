import matplotlib.pyplot as plt
import numpy as np
import serial

arduino = serial.Serial("COM7",timeout=1, baudrate=9600)

image = np.zeros([1, 9])
image[0, 0] = 1000

# create the figure
fig = plt.figure()
ax = fig.add_subplot(111)
im = ax.imshow(image, cmap='gray', vmin=0, vmax=255)
# Create colorbar
cbar = ax.figure.colorbar(im, ax=ax)
cbar.ax.set_ylabel("Distance Scale", rotation=-90, va="bottom")
plt.show(block=False)


# draw some data in loop
while True:
    serial_output = str(arduino.readline())
    # print(serial_output)
    if "Angle" in serial_output:
        x = serial_output[9:-5]
        x = int(x)
        x = int((x/64) + 4)
        print("x:", x)
    if "Distance" in serial_output:
        distance = serial_output[12:-5]
        try:
            distance = float(distance)
        except ValueError:
            distance = 0.0
        print("dist:", distance)
    else:
        continue

    # replace the image contents
    image[0, x] = distance

    im.set_array(image)
    # redraw the figure
    fig.canvas.draw()
    fig.canvas.flush_events()