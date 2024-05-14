#ifndef FILEWRITER_INTERFACE_H_
#define FILEWRITER_INTERFACE_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <mutex>
#include <map>

using namespace std;

class FileWriter_service{

    private:
        string fileURL;
        static map<string, mutex> mutexes;

    public:
        FileWriter_service();
        FileWriter_service(string fileURL);
        ~FileWriter_service();

        //**********************************
        // DEFINICIÓN DE MÉTODOS FUNCIONALES
        //**********************************
        bool write(ostringstream& data, bool verbose = false);
        bool writeln(ostringstream& data, bool verbose = false);
        

        //**********************************************
        //* MÉTODOS DE TRATAMIENTO DE VARIABLES PRIVADAS
        //**********************************************
        string getFileURL() const;
        void setFileURL(string data);


    private:
        string calculateDefaultURL();


};


#endif