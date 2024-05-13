#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
//#include <jsoncpp/json/json.h>
#include "json_interface.hpp"

using namespace std;
//using namespace Json;



//******************************************
// DEFINICIÓN DE CONSTRUCORES Y DESTRUCTORES
//******************************************
Json_interface::Json_interface(){}
Json_interface::Json_interface(string jsonFileURL){
    setFileURL(jsonFileURL);
}

Json_interface::~Json_interface(){ }



//**********************************
// DEFINICIÓN DE MÉTODOS FUNCIONALES
//**********************************
JsonConfiguration Json_interface::getJSONConfiguration_FromFile() {

    JsonConfiguration jsonConfiguration = JsonConfiguration();
/*     
     
    ifstream file(getFileURL());
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo JSON (" << getFileURL() << ")"  << endl;
        jsonConfiguration.setStatus(false);
    }

    CharReaderBuilder builder;
    Value root;
    CharReader *reader;
    JSONCPP_STRING errs;
    bool result = true;
    
    //Leemos el fichero JSON
    reader = builder.newCharReader();
    result = parseFromStream(builder, file, &root, &errs);
    if(!result){
        cerr << "Error al parsear el archivo JSON (" << getFileURL() << ")" << endl ;
        cerr << errs << endl;
        jsonConfiguration.setStatus(false);
        return jsonConfiguration;
    }
    file.close();
    
    jsonConfiguration.getWorldConfiguration().setFitnessFunctionID(root["worldConfiguration"]["fitnessFunctionID"].asString());
    jsonConfiguration.getWorldConfiguration().setID(root["worldConfiguration"]["id"].asString());
    jsonConfiguration.getWorldConfiguration().setLimitUp(root["worldConfiguration"]["limitUp"].asDouble());
    jsonConfiguration.getWorldConfiguration().setLimitDown(root["worldConfiguration"]["limitDown"].asDouble());
    jsonConfiguration.getWorldConfiguration().setLimitRight(root["worldConfiguration"]["limitRight"].asDouble());
    jsonConfiguration.getWorldConfiguration().setLimitLeft(root["worldConfiguration"]["limitLeft"].asDouble());
   
    Value computerConfigurationSelected = Value::null;
    for (const auto &computerConfig : root["computerConfiguration"]) {
        if(checkIp(ipv4Addresses, computerConfig["IP"].asString()) || checkIp(ipv6Addresses, computerConfig["IP"].asString())){
            computerConfigurationSelected = computerConfig;
            break;
        }
    }
    if(computerConfigurationSelected == Value::null){
        cerr << "No se encontro ninguna configuración compatible con las direcciones IP del equipo." << endl ;
        jsonConfiguration.setStatus(false);
        return jsonConfiguration;
    }
    jsonConfiguration.getComputerConfiguration().setIP(computerConfigurationSelected["IP"].asString());
    jsonConfiguration.getComputerConfiguration().setLogCommonFile(computerConfigurationSelected["logCommonFile"].asString());

    RankConfiguration rankConfigurationTemp;
    for (const auto &rankConfig : computerConfigurationSelected["rankConfigurationList"]) {
        rankConfigurationTemp = RankConfiguration();
        rankConfigurationTemp.setIsDefault(false);
        rankConfigurationTemp.setOutputFile(rankConfig["outFIleURL"].asString());
        rankConfigurationTemp.setHeuristicID(rankConfig["heuristicID"].asString());
        rankConfigurationTemp.setDimensions(rankConfig["dimensions"].asUInt());
        rankConfigurationTemp.setIterations(rankConfig["iterations"].asUInt());
        rankConfigurationTemp.setPoblation(rankConfig["poblation"].asUInt());

        
        for (const auto &value : rankConfig["rankList"]) {
            rankConfigurationTemp.getRankList().push_back(value.asUInt());
        }
        for (const auto &value : rankConfig["valueList"]) {
            rankConfigurationTemp.getValueList().push_back(value.asFloat());
        }
        jsonConfiguration.getComputerConfiguration().getRankConfigurationList().push_back(rankConfigurationTemp);
    }

 */
    return jsonConfiguration;
}


//********************************************************
// ZONA DE DEFINICIÓN DE MÉTODOS DE ACCESO A LAS VARIABLES
//********************************************************
string Json_interface::getFileURL(){ return fileURL; }
void Json_interface::setFileURL(string data){ fileURL = data; }