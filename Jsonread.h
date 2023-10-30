//
// Created by amir on 17.10.23.
//

#ifndef CLIENT_JSONREAD_H
#define CLIENT_JSONREAD_H

#include <json/json.h>
#include <iostream>
#include <fstream>
#include <map>
#include <list>

struct NetworkSettings{
    std::string ip;
    uint32_t tcpPort;
};


void parseConfigFile(std::map<int, int>& din,
                     std::map<int, int>& dout,
                     std::map<int, int>& ain,
                     std::map<int, int>& aout) {

    std::ifstream ifs("./MONITORING_VARS.txt");
    Json::Reader reader;
    Json::Value obj;

    std::map<Json::Value, int> vars;

    reader.parse(ifs, obj); // reader can also read strings

    int ind = 0;
    for (auto &it: obj["varList"]) {
        ind = ind + 4;
        if (it["portValue"].asInt() > 0 && it["portValue"].asInt() < 8) {
            vars.insert({it, ind});
//            std::cout << it["inOutType"] << std::endl;
        }
        if (it["portValue"].asInt() >= 8)
            std::cout << "ERROR: portValue must be: 0 <= portValue <= 8" << std::endl;
    }


    for (auto &[val, ind]: vars) {
        if (val["type"] == "BOOL") {
            if (val["inOutType"] == "INPUT")
                din.insert({val["portValue"].asInt(), ind});
            else if (val["inOutType"] == "OUTPUT")
                dout.insert({val["portValue"].asInt(), ind});
        } else if (val["type"] == "LREAL")
            if (val["inOutType"] == "INPUT")
                ain.insert({val["portValue"].asInt(), ind});
            else if (val["inOutType"] == "OUTPUT")
                aout.insert({val["portValue"].asInt(), ind});
    }

}
    /// по завершению цикла, в векторах будут храниться адреса ячеек, куда следует записывать или откуда считывать

//    for (auto it: din) {
//        std::cout << it.first << " - " << it.second << " din ";
//    }
//    std::cout << std::endl;
//    for (auto it: dout) {
//        std::cout << it.first << " - " << it.second << " dout ";
//    }
//    std::cout << std::endl;
//    for (auto it: ain) {
//        std::cout << it.first << " - " << it.second << " ain ";
//    }
//    std::cout << std::endl;
//    for (auto it: aout) {
//        std::cout << it.first << " - " << it.second << " aout ";
//    }
//    std::cout << std::endl;
//}

int parseJsonConfig(NetworkSettings& settings){
    std::ifstream ifs("./modbus_config.json");
    Json::Reader reader;
    Json::Value obj;

    if(!ifs.is_open()){
        std::cout << "Can't open file" << std::endl;
        return -1;
    }

    reader.parse(ifs, obj);
    settings.ip = obj["ip"].asString();
    settings.tcpPort = obj["port"].asInt();

    return 0;
}

#endif //CLIENT_JSONREAD_H
