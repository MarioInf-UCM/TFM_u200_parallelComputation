#ifndef JSONCONFIGURATION_H_
#define JSONCONFIGURATION_H_

#include <string>
#include <vector>
#include "test/test.hpp"


using namespace std;

class JsonConfiguration{
    
    private:
        bool status;
        string outDir;
        string logFile;
        vector<Test> testList;
    

    public:
        JsonConfiguration();
        JsonConfiguration(bool status, string outDir, string logFile, vector<Test> testList);
        ~JsonConfiguration();


        string displayInfo(string initLineString);


        bool get_status() const;
        void set_status(bool data);

        string get_outDir() const;
        void set_outDir(string data);

        string get_logFile() const;
        void set_logFile(string data);

        vector<Test>& get_testList();
        Test getElement_testList_byIndex(unsigned int index);
        void set_testList(vector<Test> newList);
        void setElement_testList_byIndex(unsigned int index, Test value);

};

#endif