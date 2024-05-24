#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def generate_plot(csv_url, image_location):
    try:
        data = pd.read_csv(csv_url)
        x = data['x']
        y1 = data['CPU time execution']
        y2 = data['Device time execution']
        y3 = data['Send buffers to device time']
        y4 = data['Receive buffers from device time']
    except FileNotFoundError:
        print(f"File '{csv_url}' not found.")
        return
    except KeyError:
        print(f"File '{csv_url}' does not contain necessary columns.")
        return

    plt.figure(figsize=(10, 6))
    plt.plot(x, y1, marker='o', label='CPU time execution')
    plt.plot(x, y2, marker='o', label='Device time execution')
    plt.plot(x, y3, marker='o', label='Send buffers to device time')
    plt.plot(x, y4, marker='o', label='Receive buffers from device time')

    plt.title(f'Execution Time Comparison - {os.path.basename(csv_url)}')
    plt.xlabel('Data size')
    plt.ylabel('Time (ms)')
    plt.legend()
    plt.xscale('log')
    plt.yscale('log')


    image_locationTemp = image_location +".jpg"
    plt.savefig(image_locationTemp, format='jpg')

    image_locationTemp = image_location +".eps"
    plt.savefig(image_locationTemp, format='eps')

    plt.close()

    


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: ./generatePictures.py <csv_url> <image_location>")
        sys.exit(1)

    csv_url = sys.argv[1]
    image_location = sys.argv[2]

    if not os.path.exists(image_location):
        os.makedirs(image_location)

    generate_plot(csv_url, image_location)
    print("Plot generated and saved as a JPEG image.")
