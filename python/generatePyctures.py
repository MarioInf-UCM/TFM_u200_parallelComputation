#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def generate_plot(csv_url):
    try:
        data = pd.read_csv(csv_url)
        x = data['x']
        y1 = data['CPU time execution']
        y2 = data['CPU time execution optimizated']
        y3 = data['Device time execution']
        y4 = data['Transmision (S+R) time']
    except FileNotFoundError:
        print(f"File '{csv_url}' not found.")
        return
    except KeyError:
        print(f"File '{csv_url}' does not contain necessary columns.")
        return

    # Define the order of the categories
    categories = ['mini', 'small', 'medium', 'large', 'extralarge']

    plt.figure(figsize=(10, 6))
    plt.plot(x, y1, marker='o', label='Ejecucion de la CPU', color='red')
    plt.plot(x, y2, marker='o', label='Ejecución de la CPU paralelizada', color='darkred')
    plt.plot(x, y3, marker='o', label='Ejecución del dispositivo', color='blue')
    plt.plot(x, y4, marker='o', label='Transmisión y recepción de datos al dispositivo', color='aquamarine')

    plt.title(f'Comparación de tiempos de ejecución - {os.path.basename(csv_url)}')
    plt.xlabel('Tamaño de datos de entrada (según Polybench/C)')
    plt.ylabel('Tiempo (ms)')
    plt.legend()
    plt.xticks(ticks=range(len(categories)), labels=categories)
    plt.yscale('log')

    #for i in range(len(x)):
    #    plt.annotate(f'{y1[i]:.2f}', (x[i], y1[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='red', edgecolor='black', boxstyle='round,pad=0.2'))
    #    plt.annotate(f'{y2[i]:.2f}', (x[i], y2[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='darkred', edgecolor='black', boxstyle='round,pad=0.2'))
    #    plt.annotate(f'{y3[i]:.2f}', (x[i], y3[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='blue', edgecolor='black', boxstyle='round,pad=0.2'))
    #    plt.annotate(f'{y4[i]:.2f}', (x[i], y4[i]), textcoords="offset points", xytext=(0,10), ha='center', color='white', fontsize=8, bbox=dict(facecolor='skyblue', edgecolor='black', boxstyle='round,pad=0.2'))

    image_locationTemp = csv_url + ".jpg"
    plt.savefig(image_locationTemp, format='jpg', dpi=600)
    image_locationTemp = csv_url + ".pdf"
    plt.savefig(image_locationTemp, format='pdf', dpi=600)

    plt.close()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("ERROR: Entry params unexpected")
        print("Usage: ./generatePictures.py <csv_url>")
        sys.exit(1)

    csv_url = sys.argv[1]

    generate_plot(csv_url)
    print("Plot generated and saved as a JPEG image.")
