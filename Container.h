//
// Created by amir on 14.10.23.
//

#ifndef CLIENT_CONTAINER_H
#define CLIENT_CONTAINER_H

#include <vector>
#include <thread>
#include <mutex>
#include <functional>


/*!
 * Опишем простой "потокобезопасный" контейнер
 * Это разделяемый ресурс между двумя потоками, один поток фиксирует изменения и добавляет их с помощью метода add()
 * после добавления изменеий необходимо установить флаг наличия изменений setChanges()
 *
 * Поток, обрабатывающий изменения, выполняет проверку их наличия isChanged(), в случае их наличия
 * обрабатывает измения с помощью vectorProcess
 *
 */

class DigitalChannels{
public:

    /*!
     * Коструктору необходимо пердать имя нового объекта
     * @param objectName
     */
    DigitalChannels(const char *objectName):name(objectName){
        values.clear();
    }

    /*!
     * Проверка на предмет внесения изменений
     * @return bool
     */
    bool isChanged(){
        return changes;
    };

    /*!
     * Установка флага о появлении изменений
     */
    void setChanges(){
        std::lock_guard<std::mutex> locker(vectorMU);
        changes = true;
    }

    /*!
     * Снятие флага о появлении изменений
     */
    void offChanges(){
        changes = false;
    }

    /*!
     * Добавление изменеий в вектор
     * @param channel номер канала
     * @param value новое состояние (0 или 1) в канале channel
     */
    void add(int channel, int value){
        std::lock_guard<std::mutex> locker(vectorMU);
        std::cout << name << " adding " << channel << "- " << value << std::endl;
        values.emplace_back(channel, value);
    }

    /*!
     * Функция ообработки изменений
     * @param func необходимо передать функтор описывающий как именно обработать пары <channel, value>
     */
    void vectorProcess(std::function<void(int, int)> func){
        std::lock_guard<std::mutex> locker(vectorMU);
        for (auto& it: values) {
            func(it.first, it.second);
        }
        values.clear();
        offChanges();
    }

    bool empty(){
        return values.empty();
    }

    void printContent(){
        std::lock_guard<std::mutex> locker(vectorMU);
        std::cout << this->name << std::endl;
        for (auto& it: values){
            std::cout << it.first << " - " << it.second << std::endl;
        }
    }

private:
    std::vector<std::pair<int, int>> values;
    std::mutex vectorMU;
    bool changes = false;
    std::string name;

};

#endif //CLIENT_CONTAINER_H
