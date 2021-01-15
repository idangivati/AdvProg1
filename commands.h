

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
class UploadTimeS: public Command{
    string description = "1.upload a time series csv file\r\n";
public:
    UploadTimeS(DefaultIO* dio) : Command (dio) {}
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
class AlgoSet: public Command{
    string description = "2.algorithm settings\r\n";
    float threshold = 0.9;
public:
    AlgoSet(DefaultIO* dio) : Command (dio) {}
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
    float getThreshold(){
        return this->threshold;
    }
};

class DetectAnomaly: public Command {
    string description = "3.detect anomalies\r\n";
    HybridAnomalyDetector detector;
    vector<AnomalyReport> report;
public:
    DetectAnomaly(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute() {
        detector.learnNormal(TimeSeries("anomalyTrain.csv"));
        report = detector.detect(TimeSeries("anomalyTest.csv"));
        dio->write("anomaly detection complete.\r\n");
    }
    vector<AnomalyReport>* getReport() {
        return &this->report;
    }
    void setThreshold(float threshold){
        detector = HybridAnomalyDetector(threshold);
    }
};

class ShowResults: public Command {
    string description = "4.display results\r\n";
    vector<AnomalyReport> *vr;
public:
    ShowResults(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute() {
        for(AnomalyReport r: *vr){
            dio->write(std::to_string(r.timeStep) + '\t' + r.description + '\n');
        }
        dio->write("Done.\r\n");
    }
    void setReport(vector<AnomalyReport>* report){
        this->vr = report;
    }
};

class UploadAnomaly: public Command{
    string description = "5.upload anomalies and analyze results\r\n";
    vector<AnomalyReport> *vr;
public:
    UploadAnomaly(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){

    }
    void setReport(vector<AnomalyReport>* report){
        this->vr = report;
    }
};

class Finish: public Command {
    string description = "6.exit\r\n";
public:
    Finish(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){

    }
};


#endif /* COMMANDS_H_ */
