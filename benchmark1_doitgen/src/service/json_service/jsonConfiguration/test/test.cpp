#include "test.hpp"


//*******************************
// CONSTRUCTORS DEFINITION ZONE *
//*******************************
Test::Test():
    statsFile(""),
    executionList(vector<Execution>()
){}
Test::Test(string statsFile, vector<Execution> executionList):
    statsFile(statsFile),
    executionList(vector<Execution>()
){
    set_executionList(executionList);
}
Test::~Test(){}



//************************************
// DEFINICIÓN DE MÉTODOS FUNCIONALES *
//************************************
string Test::displayInfo(string initLineString){
    string info = initLineString + "{\n";
    
    info += initLineString + "Stats File: " + get_statsFile() + "\n";
    info += initLineString + "Execution List:\n";
    for (size_t i = 0; i < get_executionList().size(); ++i) {
        info += get_executionList()[i].displayInfo(initLineString + "\t");
    }

    info += initLineString + "}\n";
    return info;
}


//*************************
//* GET AND SET FUNCTIONS *
//*************************
string Test::get_statsFile() const{ return statsFile; }
void Test::set_statsFile(string data){ statsFile=data; }


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