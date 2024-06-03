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
        string name;
        string statsFile;
        bool generatePictures;
        vector<Execution> executionList;



    //*****************************
    //* DEFINITION ZONE FUNCTIONS *
    //*****************************
    public:
        Test();
        Test(string statsFile, vector<Execution> executionList);
        ~Test();


        string displayInfo(string initLineString);



        //*************************
        //* GET AND SET FUNCTIONS *
        //*************************
        string get_name() const;
        void set_name(string data);

        string get_statsFile() const;
        void set_statsFile(string data);

        bool get_generatePictures() const;
        void set_generatePictures(bool data);
        
        vector<Execution>& get_executionList();
        Execution getElement_executionList_byIndex(unsigned int index);
        void set_executionList(vector<Execution> newList);
        void setElement_executionList_byIndex(unsigned int index, Execution value);






};

#endif