#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}
/**
 * This function start the menu and connect
 */
void CLI::start() {
    bool check = 1;
    int size = 6;
    float choice = 0;
    int convChoice;
    Command** menu = new Command*[size];
    UploadTimeS* uploadTimeS = new UploadTimeS(dio);
    AlgoSet* algoSet = new AlgoSet(dio);
    DetectAnomaly* detectAnomaly = new DetectAnomaly(dio);
    ShowResults* showResults = new ShowResults(dio);
    UploadAnomaly* uploadAnomaly = new UploadAnomaly(dio);
    detectAnomaly->setThreshold(algoSet->getThreshold());
    showResults->setReport(detectAnomaly->getReport());
    uploadAnomaly->setReport(detectAnomaly->getReport());
    uploadAnomaly->setN(uploadTimeS->getN());
    menu[0] = uploadTimeS;
    menu[1] = algoSet;
    menu[2] = detectAnomaly;
    menu[3] = showResults;
    menu[4] = uploadAnomaly;
    menu[5] = new Finish(dio);
    while(check) {
        // the menu
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < size; i++) {
            menu[i]->cDescription();
        }
        dio->read(&choice);
        if (choice < 1 || choice > 6 || floorf(choice) != choice) {
            // we the user pressed a number that is not on the menu.
            check = false;
        }
        else {
            convChoice = (int) choice;
            if (convChoice == 6) { // if the user pressed exit.
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

