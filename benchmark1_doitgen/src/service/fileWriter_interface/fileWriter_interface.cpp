#include <iostream>
#include <fstream>
#include <string>

#include "fileWriter_interface.hpp"

using namespace std;


//***************************************************************
// DEFINICIÓN DE CONSTRUCORES, DESTRUCTORES Y VARIABLES ESTÁTICAS
//***************************************************************
map<string, mutex> FileWriter_interface::mutexes;

FileWriter_interface::FileWriter_interface():
    fileURL(calculateDefaultURL())
{}
FileWriter_interface::FileWriter_interface(string fileURL):
    fileURL(fileURL)
{}

FileWriter_interface::~FileWriter_interface(){}



//**********************************
// DEFINICIÓN DE MÉTODOS FUNCIONALES
//**********************************
bool FileWriter_interface::write(ostringstream& data, bool verbose){
    lock_guard<mutex> lock(mutexes[fileURL]);
    ofstream archivo(fileURL, ios::app);

    if (archivo.is_open()) {

        archivo << data.str() << ends;
        if(verbose){
            cout << data.str() << ends;
        }
        data.clear();
        (data.str(std::string()), data.clear());

    } else {
        cerr << "El archivo de escritura no se abrió correctamente." << endl;
        return false;
    }

    return true;
}

bool FileWriter_interface::writeln(ostringstream& data, bool verbose){
    lock_guard<mutex> lock(mutexes[fileURL]);
    ofstream archivo(fileURL, ios::app);

    if (archivo.is_open()) {

        archivo << data.str() << endl;
        if(verbose){
            cout << data.str() << endl;
        }
        data.clear();
        (data.str(std::string()), data.clear());

    } else {
        cerr << "El archivo de escritura no se abrió correctamente." << endl;
        return false;
    }
    return true;
}





//**********************************************
//* MÉTODOS DE TRATAMIENTO DE VARIABLES PRIVADAS
//**********************************************
string FileWriter_interface::getFileURL() const{ return fileURL; }
void FileWriter_interface::setFileURL(string data){ 
    fileURL = data; 
}


//******************
//* MÉTODOS PRIVADOS
//******************
string FileWriter_interface::calculateDefaultURL() {
    time_t tiempoActual = time(0);
    tm* tiempoLocal = localtime(&tiempoActual);
    char buffer[80];
    strftime(buffer, 80, "DefaultName_%Y-%m-%d_%H-%M-%S", tiempoLocal);
    return string(buffer);
}