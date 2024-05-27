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
        y2 = data['CPU time execution optimizated']
        y3 = data['Device time execution']
        y4 = data['Send buffers to device time']
        y5 = data['Receive buffers from device time']
    except FileNotFoundError:
        print(f"File '{csv_url}' not found.")
        return
    except KeyError:
        print(f"File '{csv_url}' does not contain necessary columns.")
        return

    plt.figure(figsize=(10, 6))
    plt.plot(x, y1, marker='o', label='CPU time execution', color='red')
    plt.plot(x, y2, marker='o', label='CPU time execution optimizated', color='darkred')
    plt.plot(x, y3, marker='o', label='Device time execution', color='blue')
    plt.plot(x, y4, marker='o', label='Send buffers to device time', color='skyblue')
    plt.plot(x, y5, marker='o', label='Receive buffers from device time', color='aquamarine')

    plt.title(f'Execution Time Comparison - {os.path.basename(csv_url)}')
    plt.xlabel('Data size')
    plt.ylabel('Time (ms)')
    plt.legend()
    plt.xscale('log')
    plt.yscale('log')

for i in range(len(x)):
    plt.annotate(f'{y1[i]:.2f}', (x[i], y1[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='red', edgecolor='black', boxstyle='round,pad=0.2'))
    plt.annotate(f'{y2[i]:.2f}', (x[i], y2[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='darkred', edgecolor='black', boxstyle='round,pad=0.2'))
    plt.annotate(f'{y3[i]:.2f}', (x[i], y3[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='blue', edgecolor='black', boxstyle='round,pad=0.2'))
    plt.annotate(f'{y4[i]:.2f}', (x[i], y4[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='skyblue', edgecolor='black', boxstyle='round,pad=0.2'))
    plt.annotate(f'{y5[i]:.2f}', (x[i], y5[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='aquamarine', edgecolor='black', boxstyle='round,pad=0.2'))



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
