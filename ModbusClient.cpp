//
// Created by amir on 14.10.23.
//
#include "ModbusClient.h"

ModbusClient::ModbusClient(std::string serverIP, uint32_t tcpPort, DigitalChannels& DOvector_, DigitalChannels& DIvector_,
        std::map<int, int>& doAddresses_,
        std::map<int, int>& diAddresses_):
DOvector(DOvector_), DIvector(DIvector_), doAddresses(doAddresses_), diAddresses(diAddresses_){

    /// Инициализируем необходимые для работы переменные
    numbOfDO = doAddresses.size();
    numbOfDI = diAddresses.size();

    /* Allocate and initialize the memory to store the status */
//        tab_bit = (uint8_t *) malloc(numbOfDO * sizeof(uint8_t));
//        memset(tab_bit, 0, numbOfDO * sizeof(uint8_t));

    /* Allocate and initialize the memory to store the registers */
    tab_reg = (uint16_t *) malloc(ANALOG_OUTPUTS_MAX * sizeof(uint16_t));
    memset(tab_reg, 0, ANALOG_OUTPUTS_MAX * sizeof(uint16_t));

    for (const auto& [numbOfOutput, address]: doAddresses) {
        this->currentDOState[numbOfOutput] = 0;
        this->previousDOState[numbOfOutput] = 0;
    }

    ctx = modbus_new_tcp(serverIP.c_str(), tcpPort);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
    }

}

void ModbusClient::modbusPolling(){
    std::cout << "Modbus Polling  "<< std::endl;
    int rc = 0;

    if(DIvector.isChanged()){
        handleDIsignal();
    }

    int i = 0;
    for (const auto& [numbOfOutput, address]: doAddresses) {
        rc = modbus_read_bits(ctx, address, 1, &bit);  /// прочитать байт по адресу refToVector[i], записать в ячеку (tab_bit + i)
        if (rc == -1) {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
        }
        currentDOState[numbOfOutput] = bit;
    }

    for (const auto& [numbOfOutput, value]: currentDOState) {
        if (previousDOState[numbOfOutput] !=  value){
            std::cout << "Client: changes - " << numbOfOutput << " = " << (int)value << std::endl;
            DOvector.add(numbOfOutput, value);              /// add changes tj the buffer
            previousDOState[numbOfOutput] = value;
        }
    }

    if (!DOvector.empty())
        DOvector.setChanges(); ///
}

void ModbusClient::handleDIsignal(){
    std::cout << "client handler -------------------" << std::endl;
    DIvector.vectorProcess([this](int diNumb, int value){
        if (diAddresses.find(diNumb) != diAddresses.end())
            modbus_write_bit(ctx, diAddresses[diNumb], value);  /// запись необходимо производить по адресам указанным в конфиг. файле
    });
    DIvector.offChanges();
}

