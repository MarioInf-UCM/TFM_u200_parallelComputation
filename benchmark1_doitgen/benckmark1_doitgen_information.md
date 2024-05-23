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


##  WARNINGS PRODUCIDOS DUANTE LA COMPILACIÖN EN MODO SIMULACION

WARNING: [IP-LOCK-01] In your vivado project:prj for design:dr the IP bd_387c_s01tr_0 is locked. The IP is locked for the following reason - * IP definition 'SC TRANSACTION_REGULATOR (1.0)' for IP 'bd_387c_s01tr_0' (customized with software release 2021.1) has a different revision in the IP Catalog.