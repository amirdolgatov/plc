//
// Created by amir on 14.10.23.
//

#ifndef CLIENT_MODBUSCLIENT_H
#define CLIENT_MODBUSCLIENT_H

#include <iostream>
#include <thread>
#include <map>
#include <modbus.h>

#include "DIODriver.h"
#include "Container.h"


#define DISCRETE_OUTPUTS_MAX 10
#define ANALOG_OUTPUTS_MAX 10


class ModbusClient{
public:

    /*!
     * Конструктор
     * @param serverIP ip modbus сервера
     * @param tcpPort порт modbus сервера
     * @param DOvector_ потокобезопасный вектор для хранения изменений по DO
     * @param DIvector_потокобезопасный вектор для хранения изменений по DI
     * @param doAddresses_ таблица содержащая номера DO и их modbus-адреса
     * @param diAddresses_ таблица содержащая номера DI и их modbus-адреса
     */
    ModbusClient(std::string serverIP, uint32_t tcpPort,
                 DigitalChannels& DOvector_,
                 DigitalChannels& DIvector_,
                 std::map<int, int>& doAddresses_,
                 std::map<int, int>& diAddresses_);

    /*!
     * метод выполняющий периодичсекий опрос регистров modbus сервера, на предмет изменений состояния DO
     */
    void modbusPolling();

    /*!
     * метод вызывается для обработки изменений зафиксированных на DI
     */
    void handleDIsignal();

private:
    modbus_t *ctx;
    uint8_t bit;       /// coil buffer
    uint16_t *tab_reg;      /// register buffer

    /// Переменные хранятся в виде указателей, поскольку необходимо наличие конструктора по умолчанию,
    /// существование неинициализировнной ссылки неопустимо
    DigitalChannels& DOvector;   /// потокобезопасный контейнер - буфер, где хранятся изменения DO, передается обработчику DIOdriver
    DigitalChannels& DIvector;   /// потокобезопасный контейнер - буфер, где хранятся изменения DI, передается обработчику ModbusClient
    std::map<int, int>& doAddresses;  /// вектор содержит modbus адреса каждого DO, напр. адрес modbus регистра диск. выхода №4 это содержимое (*doAddresses)[3]
    std::map<int, int>& diAddresses;  /// вектор содержит modbus адреса каждого DI, напр. адрес modbus регистра диск. входа №0 это содержимое (*diAddresses)[0]

    std::map<int, uint8_t> previousDOState;
    std::map<int, uint8_t> currentDOState;
    uint8_t discreteOutputs[DISCRETE_OUTPUTS_MAX];
    uint8_t analogOutputs[ANALOG_OUTPUTS_MAX];
    int numbOfDI = 0;
    int numbOfDO = 0;
};






#endif //CLIENT_MODBUSCLIENT_H
