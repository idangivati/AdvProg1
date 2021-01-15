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
    AlgoSet* algoSet = new AlgoSet(dio);
    DetectAnomaly* detectAnomaly = new DetectAnomaly(dio);
    ShowResults* showResults = new ShowResults(dio);
    UploadAnomaly* uploadAnomaly = new UploadAnomaly(dio);
    detectAnomaly->setThreshold(algoSet->getThreshold());
    showResults->setReport(detectAnomaly->getReport());
    uploadAnomaly->setReport(detectAnomaly->getReport());
    menu[0] = new UploadTimeS(dio);
    menu[1] = algoSet;
    menu[2] = detectAnomaly;
    menu[3] = showResults;
    menu[4] = uploadAnomaly;
    menu[5] = new Finish(dio);
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
            if (convChoice == 6) {
                check = false;
            }
            else {
                //cout << "WTF" << endl;
                menu[convChoice - 1]->execute();
            }
        }
    }
}


CLI::~CLI() {

}

