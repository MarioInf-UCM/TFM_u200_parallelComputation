#ifndef _EXECUTION_HPP_
#define _EXECUTION_HPP_

#include <string>

using namespace std;

class Execution{

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:
        string kernelPackage;
        string kernel;
        int numExecutions;


    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:


        //********************************
        //* CONSTRUCTORS AND DESTRUCTORS *
        //********************************
        Execution();
        Execution(string kernelPackage, string kernel, int numExecutions);
        ~Execution();


        string displayInfo(string initLineString);



        //*************************
        //* GET AND SET FUNCTIONS *
        //*************************
        string get_kernelPackage() const;
        void set_kernelPackage(string data);

        string get_kernel() const;
        void set_kernel(string data);
        
        int get_numExecutions() const;
        void set_numExecutions(int data);
};

#endif