#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

translation_dict = {
    "CaseA execution time": "Dispositivo",
    "CaseA CPU execution time": "CPU",
    "CaseB execution time": "Dispositivo",
    "CaseB CPU execution time": "CPU",
    "CaseC execution time": "Dispositivo",
    "CaseC CPU execution time": "CPU",
}


def generate_plot(csv_url):
    try:
        data = pd.read_csv(csv_url)
        x = data['X']
        y1 = data['CaseA CPU execution time']
        y2 = data['CaseA execution time']
        y3 = data['CaseB CPU execution time']
        y4 = data['CaseB execution time']
        y5 = data['CaseC CPU execution time']
        y6 = data['CaseC execution time']
    except FileNotFoundError:
        print(f"GenarateGraphics_performance.py - ERROR..: File '{csv_url}' not found.")
        return

    # Define the order of the categories
    #categories = ['mini', 'small', 'medium', 'large', 'extralarge']
    categories = ['2Gb', '4Gb', '8Gb', '16Gb', '32Gb']                  #Categorias exclusivas del kernel VectorAdd

    plt.figure(figsize=(10, 7))
    plt.plot(x, y1, marker='o', label='Ejecución CPU (n)', color='lightcoral')
    plt.plot(x, y2, marker='s', label='Ejecución Dispositivo (n)', color='lightblue')
    plt.plot(x, y3, marker='o', label='Ejecución CPU 10*(n)', color='red')
    plt.plot(x, y4, marker='s', label='Ejecución Dispositivo 10*(n)', color='blue')
    plt.plot(x, y5, marker='o', label='Ejecución CPU 100*(n)', color='darkred')
    plt.plot(x, y6, marker='s', label='Ejecución Dispositivo 100*(n)', color='darkblue')

    plt.title(f'Comparación de tiempos de ejecución - {os.path.basename(csv_url)}')
    #plt.xlabel('Tamaño de datos de entrada (según Polybench/C)')
    plt.xlabel('Tamaño de datos de entrada')                            #Etiqueta exclusivas del kernel VectorAdd

    plt.ylabel('Tiempo (ms)')
    plt.legend()
    plt.xticks(ticks=range(len(categories)), labels=categories)
    plt.yscale('log')


    image_locationTemp = csv_url + ".jpg"
    plt.savefig(image_locationTemp, format='jpg', dpi=600)
    image_locationTemp = csv_url + ".pdf"
    plt.savefig(image_locationTemp, format='pdf', dpi=600)

    plt.close()


def generateTable_data(csv_file):
    
    df=DataFrame()
    dfIni = pd.read_csv(csv_file)
    dfIni = df.transpose()    
    dfIni.columns = df.iloc[0]
    df.index = df.index.map(lambda x: translation_dict.get(x, x))       # Traducir los índices (valores de la primera columna



    #newEmptyRow_name = "\\textbf{{\\emph{{\\underline{{O(n)}}}}}}"
    #newEmptyRow = df.iloc[0, 1:].copy()
    #newEmptyRow.loc[:]=""
    #df = pd.concat([df.iloc[0], pd.DataFrame([newEmptyRow], index=[newEmptyRow_name]), df.iloc[1,:]])

    



    # Redondear los valores numéricos a dos decimales y agregar tabulador a las dos últimas columnas
    def format_value(x, col):
        if isinstance(x, (int, float)):
            x = f"{x:.2f}"
        if col in df.columns[-2:]:  # Las dos últimas columnas
            x = f"\t{x}"
        return x

    df = df.apply(lambda col: col.map(lambda x: format_value(x, col.name)))

    # Crear el contenido LaTeX para la tabla con colores
    latex_table = "\\begin{table}[H]\n"
    latex_table += "    \\centering\n"
    latex_table += "    \\begin{tabular}{lllllll}\n"
    
    numRow=0
    for idx, row in df.iterrows():
        if numRow == 0:
            latex_table += f"    \\rowcolor[HTML]{{DAE8FC}} \\ & "
            latex_table += " & ".join(" \\textbf{"+row+"}") + " \\\\\n"
        elif numRow%2 != 0:
            if idx == "CPU -> Device (ejec)" or idx == "CPU -> Device (ejec+Tras)":
                latex_table += f"    \\cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
                latex_table += " & ".join(row+"\%") + " \\\\\n"            
            else:
                latex_table += f"    \\cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
                latex_table += " & ".join(row+"ms") + " \\\\\n"
        else:
            if idx == "CPU -> Device (ejec)" or idx == "CPU -> Device (ejec+Tras)":
                latex_table += f"    \\rowcolor[HTML]{{EFEFEF}} \cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
                latex_table += " & ".join(row+"\%") + " \\\\\n"
            else:
                latex_table += f"    \\rowcolor[HTML]{{EFEFEF}} \cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
                latex_table += " & ".join(row+"ms") + " \\\\\n"            
        numRow+=1
    
    nameFile = ultimo_elemento = csv_file.split("/")[-1]
    nameFile_parts=nameFile.split("_")


    latex_table += "    \\end{tabular}\n"
    latex_table += f"    \\caption[Resultados de rendimiento "+nameFile_parts[1]+" "+nameFile_parts[2]+"]{{Resultados de rendimiento "+nameFile_parts[1]+" "+nameFile_parts[2]+"}}\n"
    latex_table += f"    \\label{{table_{os.path.splitext(os.path.basename(csv_file))[0]}}}\n"
    latex_table += "\\end{table}"

    # Crear el nombre del archivo de salida .tex
    output_tex_file = f"{csv_file}.tex"

    # Escribir el contenido LaTeX en un archivo
    with open(output_tex_file, 'w') as f:
        f.write(latex_table)





if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("GenarateGraphics_performance.py - ERROR..: ERROR: Entry params unexpected")
        print("Usage: ./generatePictures.py <csv_url>")
        sys.exit(1)

    csv_url = sys.argv[1]

    generate_plot(csv_url)
    print("Plot generated and saved as a JPEG image.")
    generateTable_data(csv_url)
    print("Data tagble generated and save as TEX file.")
