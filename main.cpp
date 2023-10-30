
#include "DIODriver.h"
#include "ModbusClient.h"
#include "Jsonread.h"

#define DISCRETE_OUTPUTS_MAX 8
#define ANALOG_OUTPUTS_MAX 8

#define NB_CONNECTION 5

int main(int argc, char *argv[]) {

    DigitalChannels DIvector("Digital Inputs");
    DigitalChannels DOvector("Digital Outputs");

    std::map<int, int> diAddresses;
    std::map<int, int> doAddresses;
    std::map<int, int> ainAddresses;
    std::map<int, int> aoutAddresses;

    NetworkSettings settings;

    parseJsonConfig(settings);
    parseConfigFile(diAddresses, doAddresses, ainAddresses, aoutAddresses);

    ModbusClient client = ModbusClient(settings.ip, settings.tcpPort, DOvector, DIvector, doAddresses, diAddresses);
    DIOdriver driver = DIOdriver(8, DOvector, DIvector);
    std::thread th1([&]() {
        for (;;) {
            usleep(100000);
            client.modbusPolling();
        }
    });
    th1.detach();
    std::thread th2([&]() {
        for (;;) {
            usleep(100000);
            driver.DIpolling();
        }
    });
    th2.detach();
    getchar();

    return 0;
}

