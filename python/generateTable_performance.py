import pandas as pd
import numpy as np
import os
import sys

# Diccionario para traducir los valores de la primera columna
translation_dict = {
    "CPU execution time": "CPU",
    "CPU optimized execution time": "CPU opt",
    "Device execution time": "Dispositivo (Eje)",
    "Transmision (S+R) time": "Trasmisión (Env+Rec)",
    "Send to device time": "Envío",
    "Recieve from device time": "Recepción"
}


def Speed_CPU_device(row):
    try:
        valor_b = float(row.iloc[1])
        valor_d = float(row.iloc[3])
        
        resultado = ((valor_b - valor_d) / valor_b) * 100
        return resultado
    except (ValueError, ZeroDivisionError):
        return np.nan


def csv_to_latex(csv_file):
    # Leer el archivo CSV
    df = pd.read_csv(csv_file)
    
    # Transponer el DataFrame
    df = df.transpose()
    
    # Ajustar los nombres de las columnas para que se muestren como el índice después de transponer
    df.columns = df.iloc[0]
    df.index = df.index.map(lambda x: translation_dict.get(x, x))       # Traducir los índices (valores de la primera columna


    #Agregamos fila: Tiempo total del dispositivo
    newRow_name = "Dispositivo (Ejec+Tras)"
    newRow = df.iloc[3] - df.iloc[4]
    df = pd.concat([df.iloc[:4], pd.DataFrame([newRow], index=[newRow_name]), df.iloc[4:]])

    #Agregamos fila: Aceleracion CPU -> Dispositivo (Ejec.)
    newRow = ((df.iloc[1] - df.iloc[3]) / df.iloc[1])*100;
    newRow.name = "CPU -> Device (ejec)"
    df = df._append(newRow)

    #Agregamos fila: Aceleracion CPU -> Dispositivo (Ejec.+Tras.)
    newRow = ((df.iloc[1] - df.iloc[4]) / df.iloc[1])*100;
    newRow.name = "CPU -> Device (ejec+Tras)"
    df = df._append(newRow)

    
    df.index = df.index.map(lambda x: translation_dict.get(x, x))       # Traducir los índices (valores de la primera columna)

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
            if {idx} == "CPU -> Device (ejec)":
                latex_table += f"    \\cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
                latex_table += " & ".join(row) + "\% \\\\\n"            
            else:
                latex_table += f"    \\cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
                latex_table += " & ".join(row) + " \\\\\n"
        else:
            latex_table += f"    \\rowcolor[HTML]{{EFEFEF}} \cellcolor[HTML]{{DAE8FC}} \\textbf{{{idx}}} & "
            latex_table += " & ".join(row) + " \\\\\n"
        numRow+=1
    
    latex_table += "    \\end{tabular}\n"
    latex_table += f"    \\caption[Datos obtenidos de los diferentes tamaños]{{Datos obtenidos de los diferentes tamaños}}\n"
    latex_table += f"    \\label{{table_{os.path.splitext(os.path.basename(csv_file))[0]}}}\n"
    latex_table += "\\end{table}"

    # Crear el nombre del archivo de salida .tex
    base_name = os.path.splitext(os.path.basename(csv_file))[0]
    output_tex_file = f"{base_name}.tex"

    # Escribir el contenido LaTeX en un archivo
    with open(output_tex_file, 'w') as f:
        f.write(latex_table)

if __name__ == "__main__":
    # Asegurarse de que se pase un argumento
    if len(sys.argv) != 2:
        print("Uso: python script.py ruta/al/archivo.csv")
        sys.exit(1)

    # Obtener la ruta del archivo CSV desde los argumentos de línea de comandos
    csv_file = sys.argv[1]
    csv_to_latex(csv_file)
