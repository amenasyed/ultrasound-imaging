import matplotlib.pyplot as plt
import numpy as np
import serial

arduino = serial.Serial("COM7",timeout=1, baudrate=9600)

xRes = 17
yRes = 17
y = yRes

max_dist = 1000
image = np.zeros([yRes, xRes])
image[0, xRes-1] = 1000

# create the figure
fig = plt.figure()
ax = fig.add_subplot(111)
im = ax.imshow(image, cmap='gray', vmin=0, vmax=255)
# Create colorbar
cbar = ax.figure.colorbar(im, ax=ax)
cbar.ax.set_ylabel("Distance Scale", rotation=-90, va="bottom")
plt.show(block=False)


# draw some data in loop
while y >= 0:
    fig.canvas.draw()
    fig.canvas.flush_events()
    
    serial_output = str(arduino.readline())
    # print(serial_output)
    if "Y" in serial_output:
        y = serial_output[5:-5]
        y = int(y)
        y = (yRes - 1) - y
        print("y:", y)
        continue
    if "X" in serial_output:
        x = serial_output[5:-5]
        x = int(x)
        # Flip the order in which the row populates since the scan direction flip-flops.
        # if y % 2 == 0:
            # x = (xRes - 1) - x
        x = (xRes - 1) - x
        print("x:", x)
    if "Z" in serial_output:
        distance = serial_output[5:-5]
        try:
            distance = float(distance)
            print("Dist:", distance)
        except ValueError:
            distance = max_dist
            print("Dist out of range")
    else:
        continue

    # replace the image contents
    image[(yRes - 1) - y, x] = distance

    # redraw the figure
    im.set_array(image)

    if y == 0 and x == 0:
        print(image)
    
