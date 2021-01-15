#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start() {
    bool check = 1;
    int size = 6;
    float choice = 0;
    int convChoice;
    Command** menu = new Command*[size];
    menu[0] = new uploadTimeS(dio);
    menu[1] = new algoSet(dio);
    menu[2] = new detectAnomaly(dio);
    menu[3] = new showResults(dio);
    menu[4] = new uploadAnomaly(dio);
    menu[5] = new finish(dio);
    /*dio->write("Welcome to the Anomaly Detection Server.\r\n");
    dio->write("Please choose an option:\r\n");
    //cout << dio->read() << endl;
    for (int i = 0; i < size; i++) {
        menu[i]->cDescription();
    }*/
    while(check) {
        dio->write("Welcome to the Anomaly Detection Server.\r\n");
        dio->write("Please choose an option:\r\n");
        for (int i = 0; i < size; i++) {
            menu[i]->cDescription();
        }
        dio->read(&choice);
        if (choice < 1 || choice > 6 || floorf(choice) != choice) {
            check = false;
        }
        else {
            convChoice = (int) choice;
            if (convChoice == 6){
                check = false;
            }
            else {
                menu[convChoice - 1]->execute();
            }
        }
    }
}


CLI::~CLI() {

}

