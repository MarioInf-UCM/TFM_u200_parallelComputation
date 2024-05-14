#ifndef _TEST_HPP_
#define _TEST_HPP_

#include <string>
#include <vector>
#include "../execution/execution.hpp"

using namespace std;

class Test{

    //*****************************
    //* DEFINITION ZONE ATRIBUTES *
    //*****************************
    private:
        string statsFile;
        string logFile;
        vector<Execution> executionList;



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        Test();
        Test(string statsFile, string logFile, vector<Execution> executionList);
        ~Test();


        string displayInfo();



        //*************************
        //* GET AND SET FUNCTIONS *
        //*************************
        string get_statsFile() const;
        void set_statsFile(string data);

        string get_logFile() const;
        void set_logFile(string data);
        
        vector<Execution>& get_executionList();
        Execution getElement_executionList_byIndex(unsigned int index);
        void set_executionList(vector<Execution> newList);
        void setElement_executionList_byIndex(unsigned int index, Execution value);






};

#endif