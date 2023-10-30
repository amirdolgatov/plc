//
// Created by amir on 12.10.23.
//

#include "DIODriver.h"

void DIOdriver:: set(int n){
        int8_t mask = 1;
        word |= (mask << n);
        std::cout << "set " << (int)word << std::endl;
    }
void DIOdriver::off(int n){
        uint8_t mask = 1;
        word &= ~(mask << n);
        std::cout << "off " << (int)word << std::endl;
    }

void DIOdriver::writeToHardware(){
//        std::cout << "DO WORD -> " << (int)word << std::endl;
        writeDOcommand = leftPart + std::to_string(word) + rightPart; /// вставка регистра для записи в DO
        system(writeDOcommand.c_str());  /// передача команды в шелл
    }

void DIOdriver::handleChanges(){
    std::cout << "driver handler -------------------" << std::endl;
//    DOvector->printContent();
    DOvector.vectorProcess([this](int do_numb, int value){
        switch(value){
            case 0: off(do_numb);
                break;
            case 1: set(do_numb);
                break;
            default: std::cout << "handleChanges: Wrong DO value" << std::endl;
        }
    });
    DOvector.offChanges();
    writeToHardware();
    }

void DIOdriver::DIpolling(){
//    std::cout << "DIpolling started! "<< std::endl;
    if (DOvector.isChanged()){
        handleChanges();
    }

    scriptRet = popen(readDIcommand.c_str(), "r");
    lastchar = fread(result, 1, 10, scriptRet);
    result[lastchar] = '\0';
    pclose(scriptRet);

    diState = atoi(result);
//    std::cout << "DIstate " << diState << std::endl;
    if(diState != diStatePrevious){               /// текущий регистр состония DI отличается от предыдущего
        uint16_t temp = diState^diStatePrevious;  /// XOR - фиксируем биты которые различаются
        uint16_t mask = 1;
        for(int i = 0; i < diNumb; i++){
            if(temp & (mask << i))               /// если это бит значение которого изменилось
                DIvector.add(i, (diState & (mask << i)) >> i );   /// взять новое значение бита, занести в буфер изменений
            }
        diStatePrevious = diState;
        DIvector.setChanges();

        }
    }


