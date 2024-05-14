#ifndef JSONCONFIGURATION_H_
#define JSONCONFIGURATION_H_

#include <string>
#include <vector>
#include "test/test.hpp"


using namespace std;

class JsonConfiguration{
    
    private:
        bool status;
        vector<Test> testList;
    

    public:
        JsonConfiguration();
        JsonConfiguration(bool status, vector<Test> testList);
        ~JsonConfiguration();


        string displayInfo();


        bool get_status() const;
        void set_status(bool data);

        vector<Test>& get_testList();
        Test getElement_testList_byIndex(unsigned int index);
        void set_testList(vector<Test> newList);
        void setElement_testList_byIndex(unsigned int index, Test value);

};

#endif