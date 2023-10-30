//
// Created by amir on 12.10.23.
//

#ifndef CLIENT_DIODRIVER_H
#define CLIENT_DIODRIVER_H

#include<vector>
#include<iostream>
#include<mutex>
#include<string>
#include<thread>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "Container.h"


class DIOdriver{

public:

    /// Конструктор принимает указатель на функцию обработчик
    DIOdriver(uint8_t numbOfInputs, DigitalChannels& DOvector_, DigitalChannels& DIvector_):diNumb(numbOfInputs),
    DOvector(DOvector_), DIvector(DIvector_)
    {
        word = 0;
        writeToHardware();   /// на старте программы обнуляем цифровые выходы
    }


    /*!
     * "Включить" бит в байте word
     * @param n номер бита
     */
    void set(int n);

    /*!
     * "Выключить" бит в байте word
     * @param n номер бита
     */
    void off(int n);

    /*!
     * Записать байи word в регистр состояния DO
     */
    void writeToHardware();

    /*!
     * Обработка вектора изменений DOchanges, запись изменений в DO
     */

    void handleChanges();

    /*!
     * Периодический опрос состояние дискретных входов DI
     */
    void DIpolling();

private:

    /// "слово" для записи в DO
    uint8_t word = 0;
    /// Переменные для работы со скриптом опроса DI
    FILE* scriptRet;
    char result[10];
    int lastchar;
    /// Переменные для считывания регистров
    uint16_t diState = 0;
    uint16_t diStatePrevious = 0;
    /// Количество дискретных входов по умолчанию
    uint8_t diNumb = 8;

    /// Команда интерпретатору bash для чтения регистров DI/DO
    std::string leftPart = "printf \"\\x02\\x21\\x02\\\\$(printf %o ";
    std::string rightPart = ")\\\\$(printf %o 0)\" > /dev/spidev2.0";

    std::string readDIcommand = "/root/di_get.sh";
    std::string writeDOcommand;

    DigitalChannels& DOvector;
    DigitalChannels& DIvector;

};

#endif //CLIENT_DIODRIVER_H
