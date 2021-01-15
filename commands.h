

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
    virtual void cDescription() {};;
};

// implement here your command classes
class uploadTimeS: public Command{
    string description = "1.upload a time series csv file\r\n";
public:
    uploadTimeS(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){
        dio->write("Please upload your local train CSV file.\r\n");
        ofstream trainCsv ("anomalyTrain.csv");
        string trainLine = dio->read() + '\n' ;
        while (trainLine != "done\n")
        {
            trainCsv << trainLine;
            trainLine = dio->read() + '\n';
        }
        trainCsv.close();
        dio->write("Upload complete.\r\n");
        dio->write("Please upload your local test CSV file.\r\n");
        ofstream testCsv ("anomalyTest.csv");
        string testLine = dio->read() + '\n';
        while (testLine != "done\n")
        {
            testCsv << testLine;
            testLine = dio->read() + '\n';;
        }
        testCsv.close();
        dio->write("Upload complete.\r\n");
    }
};
class algoSet: public Command{
    string description = "2.algorithm settings\r\n";
    float threshold = 0.9;
public:
    algoSet(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){
        dio->write("The current correlation threshold is " + std::to_string(threshold) + "\r\n");
        dio->write("Type a new threshold\r\n");
        float nextThresh = 0;
        dio->read(&nextThresh);
        while(nextThresh < 0 || nextThresh > 1) {
            dio->write("please choose a value between 0 and 1.\r\n");
            dio->read(&nextThresh);
        }
        threshold = nextThresh;
    }
};

class detectAnomaly: public Command {
    string description = "3.detect anomalies\r\n";
    HybridAnomalyDetector detector;
    vector<AnomalyReport> report;
public:
    detectAnomaly(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute() {
        detector.learnNormal(TimeSeries("anomalyTrain.csv"));
        report = detector.detect(TimeSeries("anomalyTest.csv"));
        dio->write("anomaly detection complete.\r\n");
    }
};

class showResults: public Command {
    string description = "4.display results\r\n";
public:
    showResults(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute() {

    }
};

class uploadAnomaly: public Command{
    string description = "5.upload anomalies and analyze results\r\n";
public:
    uploadAnomaly(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){

    }
};

class finish: public Command {
    string description = "6.exit\r\n";
public:
    finish(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){

    }
};


#endif /* COMMANDS_H_ */
