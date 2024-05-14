#include "test.hpp"


//*******************************
// CONSTRUCTORS DEFINITION ZONE *
//*******************************
Test::Test():
    statsFile(""),
    logFile(""),
    executionList(vector<Execution>()
){}
Test::Test(string statsFile, string logFile, vector<Execution> executionList):
    statsFile(statsFile),
    logFile(logFile),
    executionList(vector<Execution>()
){
    set_executionList(executionList);
}
Test::~Test(){}



//************************************
// DEFINICIÓN DE MÉTODOS FUNCIONALES *
//************************************
string Test::displayInfo(){
    string info = "Stats File: " + get_statsFile() + "\n";
    info += "Log File: " + get_logFile() + "\n";
    info += "Execution List:\n";
    for (size_t i = 0; i < get_executionList().size(); ++i) {
        info += "  " + get_executionList()[i].displayInfo();
    }
    return info;
}


//*************************
//* GET AND SET FUNCTIONS *
//*************************
string Test::get_statsFile() const{ return statsFile; }
void Test::set_statsFile(string data){ statsFile=data; }

string Test::get_logFile() const{ return logFile; }
void Test::set_logFile(string data){ logFile=data; }


vector<Execution>& Test::get_executionList(){ return executionList; }
Execution Test::getElement_executionList_byIndex(unsigned int index){
    return get_executionList()[index];
}
void Test::set_executionList(vector<Execution> newList){
    get_executionList().clear();
    for (int i=0 ; i<newList.size() ; i++) {
        get_executionList().push_back(newList[i]);
    }
}
void Test::setElement_executionList_byIndex(unsigned int index, Execution value){
    get_executionList()[index] = value;
}