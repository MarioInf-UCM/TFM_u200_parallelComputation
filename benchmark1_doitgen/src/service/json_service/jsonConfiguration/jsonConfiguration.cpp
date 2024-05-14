#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "jsonConfiguration.hpp"

using namespace std;



//*******************************
// CONSTRUCTORS DEFINITION ZONE *
//*******************************
JsonConfiguration::JsonConfiguration():
    status(true),
    testList(vector<Test>()
){}
JsonConfiguration::JsonConfiguration(bool status, vector<Test> testList):
    status(status),
    testList(vector<Test>()
){
    set_testList(testList);
}

JsonConfiguration::~JsonConfiguration(){ }



//************************************
// DEFINICIÓN DE MÉTODOS FUNCIONALES *
//************************************
string JsonConfiguration::displayInfo(){
    string info = "Test List:\n";
    for (size_t i = 0; i < get_testList().size(); ++i) {
        info += "  " + get_testList()[i].displayInfo();
    }
    return info;
}



//**********************************************************
// ZONA DE DEFINICIÓN DE MÉTODOS DE ACCESO A LAS VARIABLES *
//**********************************************************
bool JsonConfiguration::get_status() const{ return status; }
void JsonConfiguration::set_status(bool data){ status = data; }


vector<Test>& JsonConfiguration::get_testList(){ return testList; }
Test JsonConfiguration::getElement_testList_byIndex(unsigned int index){
    return get_testList()[index];
}
void JsonConfiguration::set_testList(vector<Test> newList){
    get_testList().clear();
    for (int i=0 ; i<newList.size() ; i++) {
        get_testList().push_back(newList[i]);
    }
}
void JsonConfiguration::setElement_testList_byIndex(unsigned int index, Test value){
    get_testList()[index]=value;
}