

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
};

class Command{
protected:
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}
    Command() {}
	virtual void execute()=0;
	virtual ~Command(){}
    virtual void cDescription() {}
};

/**
 * A Class to upload our csv files, both train and test.
 */
class UploadTimeS: public Command{
    string description = "1.upload a time series csv file\n";
    int n = 0;
public:
    /**
     * Constructor.
     * @param dio the dio we receive from the command
     */
    UploadTimeS(DefaultIO* dio) : Command (dio) {}
    /**
     * In every command we want to write his description so we know which choice we want to pick.
     */
    virtual void cDescription() {
        dio->write(description);
    };
    /**
     * In each command we will execute the necessary
     */
    virtual void execute(){
        dio->write("Please upload your local train CSV file.\n");
        ofstream trainCsv ("anomalyTrain.csv");
        string trainLine = dio->read() + '\n' ;
        while (trainLine != "done\n")
        {
            trainCsv << trainLine;
            trainLine = dio->read() + '\n';
        }
        trainCsv.close();
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        ofstream testCsv ("anomalyTest.csv");
        string testLine = dio->read() + '\n';
        while (testLine != "done\n")
        {
            n++; // counting the rows.
            testCsv << testLine;
            testLine = dio->read() + '\n';;
        }
        n--; // we decrease by 1 for the headers we got.
        testCsv.close();
        dio->write("Upload complete.\n");
    }

    int* getN() {   // we want to get the number of rows for further calculations.
        return &this->n;
    }
};

/**
 * If we want to change the threshold and as a result change the algorithm of our correlation.
 */
class AlgoSet: public Command {
    string description = "2.algorithm settings\n";
    float threshold = 0.9;
public:
    AlgoSet(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){
        dio->write("The current correlation threshold is " + std::to_string(threshold) + "\n");
        dio->write("Type a new threshold\r\n");
        float nextThresh = 0;
        dio->read(&nextThresh);
        while(nextThresh < 0 || nextThresh > 1) { // if the user trying to put a number that is not in the range.
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&nextThresh);
        }
        threshold = nextThresh;
    }
    float *getThreshold(){
        return &this->threshold;
    }
};

/**
 * In this class we want to detect the anomaly by the files we received, train and test.
 */
class DetectAnomaly: public Command {
    string description = "3.detect anomalies\n";
    HybridAnomalyDetector detector;
    vector<AnomalyReport> report;
public:
    DetectAnomaly(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };

    /**
     * We will learn what is normal (whats suppose to build the correlation between the features).
     * After that we will detect the anomaly that we will receive from the test file.
     * In the end we will save the report we got for later use and access.
     */
    virtual void execute() {
        detector.learnNormal(TimeSeries("anomalyTrain.csv"));
        report = detector.detect(TimeSeries("anomalyTest.csv"));
        dio->write("anomaly detection complete.\n");
    }
    vector<AnomalyReport>* getReport() {
        return &this->report;
    }
    void setThreshold(float *threshold){
        detector = HybridAnomalyDetector(*threshold);
    }
};

/**
 * In this class we want to show the result of our report, feature correlation and time step.
 */
class ShowResults: public Command {
    string description = "4.display results\n";
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
        dio->write("Done.\n");
    }
    void setReport(vector<AnomalyReport>* report){
        this->vr = report;
    }
};

/**
 * In this class we checking the report we got from the user and compare it to our report we detect before.
 * We will search false report and positive reports according to the instructions.
 */
class UploadAnomaly: public Command {
    string description = "5.upload anomalies and analyze results\n";
    vector<AnomalyReport> *vr;
    int *rowNumber;
public:
    UploadAnomaly(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    void setN(int *n){
        this->rowNumber = n;
    }
    void setReport(vector<AnomalyReport>* report){
        this->vr = report;
    }
    virtual void execute(){
        vector<int> ts;
        vector<int> ourReport;
        float receiveP = 0, trueP = 0, range = 0, trueRate, falseRate, falseP = 0;
        int index = 0, N;
        vector<int> addUp;
        int currentTime = 0;
        addUp.push_back(vr->at(0).timeStep);
        for (AnomalyReport a: *vr) {
            ourReport.push_back(a.timeStep);
        }
        currentTime = vr->at(0).timeStep;
        for (int i = 0; i < vr->size() - 1; i++) { // we want to unite the ranges of our time steps by features.
            if (vr->at(i + 1).timeStep != currentTime + 1 || vr->at(i).description != vr->at(i + 1).description) {
                addUp.push_back(ourReport[i]);
                addUp.push_back(ourReport[i+1]);
            }
            currentTime = vr->at(i + 1).timeStep;
        }
        addUp.push_back(ourReport[ourReport.size() - 1]);

        dio->write("Please upload your local anomalies file.\n");
        string line = dio->read() + '\n';
        while (line != "done\n") {
            ts.push_back(0);
            for (int i = 0; line[i] != '\n'; i++) {
                if (line[i] == ',') {
                    index++;
                    ts.push_back(0);
                } else {
                    ts[index] = ts[index] * 10 + (line[i] - '0');
                }
            }
            receiveP++;
            index++;
            line = dio->read() + '\n';
        }
        dio->write("Upload complete.\n");
        for (int i = 0; i < ts.size(); i += 2) { // we want to check if there is any collusion between the reports.
            for (int k = 0; k < addUp.size() - 1; k += 2) {
                if (ts[i] <= addUp[k] && addUp[k] <= ts[i + 1] ||
                        addUp[k] <= ts[i] && ts[i] <= addUp[k + 1]) {
                    trueP++;
                    break;
                }
            }
        }
        for (int i = 0; i < ts.size() - 1; i += 2) {
            range += (ts[i + 1] - ts[i]) + 1;
        }
        // In the next calculation we following the received instructions to know the true and false positive rate.
        N = (*this->rowNumber - range);
        falseP = (addUp.size() / 2) - trueP;
        falseRate = falseP / N;
        trueRate = trueP / receiveP;
        trueRate = cut3(trueRate);
        falseRate = cut3(falseRate);
        dio->write("True Positive Rate: " + removeZero(trueRate) + '\n');
        dio->write("False Positive Rate: " + removeZero(falseRate) + '\n');
    }
    /**
     * Cutting the last three decimal digit (for example:0.05456 -> 0.05).
     * @param num the number we want to cut the last 3 decimal digits.
     * @return the number after we cut the 3 last digits according to the instruction.
     */
    float cut3 (float num){
        num = num * 1000;
        num = floorf(num);
        num = num / 1000;
        return num;
    }
    /**
     * Removing the unnecessary zeros from the decimal number we received.
     * @param num the number we want to remove the zeros from.
     * @return the string without the zeros.
     */
    string removeZero(float num) {
        //Just to remove any chance of getting 0.
        if (num == 0) {
            return "0";
        }
        string str = std::to_string(num);
        int breakPoint;
        for (int i = str.size() - 1; i >= 0; i--) {
            if (str[i] != '0' && str[i] != '.') {
                breakPoint = i;
                break;
            }
        }
        string strZ = str.substr(0, breakPoint + 1);
        return strZ;
    }
};

class Finish: public Command {
    string description = "6.exit\n";
public:
    Finish(DefaultIO* dio) : Command (dio) {}
    virtual void cDescription() {
        dio->write(description);
    };
    virtual void execute(){
    }
};


#endif /* COMMANDS_H_ */
