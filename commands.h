

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};


// you may add here helper classes


// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}

    Command() {};
	virtual void execute()=0;
	virtual ~Command(){};
    virtual void cDescription() {};
};

// implement here your command classes
class uploadTimeS: public Command{
    string description = "1. upload a time series csv file\n";
public:
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){
        dio->write("Please upload your local train CSV file.\n");
        ofstream trainCsv ("anomalyTrain.csv");
        string trainLine = dio->read();
        if (trainCsv.is_open())
        {
            while (trainLine != "done\n")
            {
                trainCsv << trainLine;
            }
            trainCsv.close();
        }
        else cout << "Unable to open file";
        ofstream testCsv ("anomalyTest.csv");
        string testLine = dio->read();
        if (testCsv.is_open())
        {
            while (testLine != "done\n")
            {
                testCsv << testLine;
            }
            testCsv.close();
        }
        else cout << "Unable to open file";
        dio->write("Upload complete.");
    }
};
class algoSet: public Command{
    string description = "2. algorithm settings\n";
    float threshold = 0.9;
public:
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){
        dio->write("The current correlation threshold is " + std::to_string(threshold));
        float nextThresh = 0;
        dio->read(&nextThresh);
        while(nextThresh < 0 || nextThresh > 1) {
            dio->write("please choose a value between 0 and 1.");
            dio->read(&nextThresh);
        }

    }
};

class detectAnomaly: public Command{
    string description = "3. detect anomalies\n";
public:
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){

    }
};

class showResults: public Command{
    string description = "4. display results\n";
public:
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){


    }
};

class uploadAnomaly: public Command{
    string description = "5. upload anomalies and analyze results\n";
public:
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){


    }
};

class finish: public Command{
    string description = "6. exit\n";
public:
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){


    }
};


#endif /* COMMANDS_H_ */
