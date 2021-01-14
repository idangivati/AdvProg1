#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start() {
    bool check = 1;
    int size = 7;
    Command *menu[size];
    menu[0] = nullptr;
    menu[1] = new uploadTimeS;
    menu[2] = new algoSet;
    menu[3] = new detectAnomaly;
    menu[4] = new showResults;
    menu[5] = new uploadAnomaly;
    menu[6] = new finish;
    dio->write("Welcome to the Anomaly Detection Server.");
    dio->write("Please choose an option:");
    cout << "WTF" << endl;
    for (int i = 1; i <= size; i++) {
        menu[i]->cDescription();
    }
}


CLI::~CLI() {

}

