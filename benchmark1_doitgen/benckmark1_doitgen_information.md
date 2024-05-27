# Benchmark 1 - Doitgen


## REQUISITOS PREVIOS

- **Instalación de la biblioteca JsonCpp:** Utilizada para poder extraer la configuración del bechmark del fichero Json.
    ```BASH
    sudo apt-get install libjsoncpp-dev
    ```
- **Instalación de Python y sus librerías pertinentes**
    ```BASH
    sudo apt-get update
    sudo apt-get install -y python3 python3-pip
    sudo apt-get install -y python3-pip
    pip3 install matplotlib
    pip install pandas
    ```


## KERNEL CPU OPTIMIZATIONS


- La directiva #pragma omp parallel for collapse(2) private(sum) permite paralelizar los bucles exteriores sobre r y q. El colapso de dos niveles de bucles (collapse(2)) permite un mayor paralelismo.
- La variable sum se declara como private para asegurar que cada hilo tenga su propia copia.
- La directiva #pragma omp simd indica al compilador que vectorice el bucle sobre s, permitiendo operaciones SIMD (Single Instruction, Multiple Data).
- Para mejorar la eficiencia de acceso a los datos, utilizamos referencias constantes para evitar múltiples llamadas a las funciones get_A(), get_C4(), y get_resultCPU().

## PRAGMAS EXPLICATIONS

Las directivas #pragma HLS utilizadas en el código especifican cómo se deben manejar los puertos y las interfaces en la síntesis de hardware, optimizando el acceso a la memoria y el control de la función. Aquí hay una explicación detallada de cada una de estas directivas:

Directivas #pragma HLS INTERFACE m_axi
Estas directivas configuran los puertos para utilizar la interfaz AXI (Advanced eXtensible Interface), que es un estándar de interconexión utilizado en FPGA para la comunicación eficiente entre módulos de hardware y la memoria.

1. #pragma HLS INTERFACE m_axi port = in1 max_write_burst_length = 32 max_read_burst_length = 32 offset = slave bundle = gmem
port = in1: Especifica que esta directiva se aplica al puerto in1, que es un puerto de entrada (read-only).
max_write_burst_length = 32: Define la longitud máxima de una operación de escritura en ráfaga a 32 datos consecutivos. Aunque in1 es de solo lectura, esta configuración puede ser redundante pero no perjudica.
max_read_burst_length = 32: Define la longitud máxima de una operación de lectura en ráfaga a 32 datos consecutivos. Esto ayuda a optimizar las transferencias de datos, mejorando el rendimiento.
offset = slave: Indica que el puerto in1 debe ser tratado como un esclavo AXI, lo que significa que responderá a las solicitudes de lectura del maestro (el hardware de control).
bundle = gmem: Agrupa este puerto en un "bundle" (grupo) llamado gmem. Los puertos en el mismo bundle pueden compartir recursos de interconexión, lo que puede ayudar en la gestión del ancho de banda y la planificación de las transferencias.
2. #pragma HLS INTERFACE m_axi port = in2 max_read_burst_length = 32 offset = slave bundle = gmem1
port = in2: Aplica la directiva al puerto in2, que también es de entrada.
max_read_burst_length = 32: Configura la longitud máxima de lectura en ráfaga a 32 datos consecutivos.
offset = slave: Configura el puerto como esclavo AXI.
bundle = gmem1: Agrupa este puerto en el bundle gmem1, permitiendo una gestión separada del puerto in1.
3. #pragma HLS INTERFACE m_axi port = out max_write_burst_length = 32 max_read_burst_length = 32 offset = slave bundle = gmem2
port = out: Aplica la directiva al puerto out, que es un puerto de salida (write-only).
max_write_burst_length = 32: Define la longitud máxima de escritura en ráfaga a 32 datos consecutivos.
max_read_burst_length = 32: Define la longitud máxima de lectura en ráfaga a 32 datos consecutivos. Aunque out es de solo escritura, esta configuración puede ser redundante pero no perjudica.
offset = slave: Configura el puerto como esclavo AXI.
bundle = gmem2: Agrupa este puerto en el bundle gmem2.
Directivas #pragma HLS INTERFACE s_axilite
Estas directivas configuran los puertos para utilizar la interfaz AXI-Lite, una versión más simple de AXI que se usa típicamente para el control de registros y operaciones de baja velocidad.

4. #pragma HLS INTERFACE s_axilite port = in1 bundle = control
port = in1: Especifica que esta directiva se aplica al puerto in1 para control.
bundle = control: Agrupa este puerto en el bundle control, que se utiliza para la gestión de las señales de control de la función.
5. #pragma HLS INTERFACE s_axilite port = in2 bundle = control
port = in2: Aplica la directiva al puerto in2 para control.
bundle = control: Agrupa este puerto en el bundle control.
6. #pragma HLS INTERFACE s_axilite port = out bundle = control
port = out: Aplica la directiva al puerto out para control.
bundle = control: Agrupa este puerto en el bundle control.
7. #pragma HLS INTERFACE s_axilite port = size bundle = control
port = size: Especifica que esta directiva se aplica al puerto size, que controla el tamaño del vector.
bundle = control: Agrupa este puerto en el bundle control.
8. #pragma HLS INTERFACE s_axilite port = return bundle = control
port = return: Aplica la directiva al valor de retorno de la función, que en este caso es void y se utiliza para finalizar la función.
bundle = control: Agrupa este puerto en el bundle control.
Resumen
Las directivas #pragma HLS INTERFACE configuran cómo los puertos de la función se mapean a las interfaces AXI y AXI-Lite, optimizando el acceso a la memoria y el control en un entorno de síntesis de hardware. Las interfaces AXI se utilizan para transferencias de datos de alta velocidad, mientras que las interfaces AXI-Lite se utilizan para el control y la configuración. Las configuraciones de longitud de ráfaga (max_write_burst_length y max_read_burst_length) ayudan a optimizar el rendimiento de las transferencias de datos, y los bundles agrupan puertos relacionados para una mejor gestión de los recursos de interconexión.


##  WARNINGS PRODUCIDOS DUANTE LA COMPILACIÖN EN MODO SIMULACION

WARNING: [IP-LOCK-01] In your vivado project:prj for design:dr the IP bd_387c_s01tr_0 is locked. The IP is locked for the following reason - * IP definition 'SC TRANSACTION_REGULATOR (1.0)' for IP 'bd_387c_s01tr_0' (customized with software release 2021.1) has a different revision in the IP Catalog.