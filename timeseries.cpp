#include "timeseries.h"
#include <string.h>

/**
 * A constructor.
 * @param CSVfileName the csv file we receive.
 */

TimeSeries::TimeSeries(const char *CSVfileName) {
    string info, first, nextLine, token;
    string delimiter = ",";
    int counterLine = 1;
    float data;
    size_t pos = 0;
    ifstream ourFile(CSVfileName);
    if(!ourFile.is_open()) {
        std::cout << "Error: file could not open" << '\n' ;
    }
    int i = 0;
    while (ourFile.good()) {
        if(counterLine == 1) {
            getline(ourFile, first);
            while ((pos = first.find(delimiter)) != std::string::npos) {
                token = first.substr(0, pos);
                titles.push_back(token);
                first.erase(0, pos + delimiter.length());
            }
            titles.push_back(first);
            counterLine++;
        } else {
            vector<float> features[titles.size()];
            while (getline(ourFile, nextLine)) {
                while ((pos = nextLine.find(delimiter)) != string::npos) {
                    token = nextLine.substr(0, pos);
                    data = stof((token));
                    features[i].push_back(data);
                    i++;
                    nextLine.erase(0, pos + delimiter.length());
                }
                //cout << "Last: " << nextLine << endl;
                features[i].push_back(stof(nextLine));
                i = 0;
            }
            for(int k  = 0; k <titles.size(); k++){
                csvMap.insert(pair<string, vector<float>>(titles[k], features[k]));
            }
        }
    }
}


/**
 * This function help us get the data of a specific feature according to his time.
 * @param feature the feature we want the data from.
 * @param time the time we want.
 * @return the data from our map according to the feature.
 */

float TimeSeries::getFeature(string feature, float time) {
    int whichTime = 0;
    for(auto element: csvMap[titles[0]]) {
        if(element == time){
            return csvMap[feature][whichTime];
        }
        whichTime++;
    }
    return -1;
}

/**
 * A function that help us add another row into the map.
 * @param newInfo the new info we want to add.
 * @return adding a row into the map.
 */

void TimeSeries::addRow(vector<float> newInfo) {
    for(int i = 0; i < (int)titles.size(); i++) {
        csvMap[titles[i]].push_back(newInfo[i]);
    }
}

/**
 * This function help us receive the features of the time series.
 * @return a vector that has the titles.
 */

vector<string> TimeSeries::getTitles() const{
    vector<string> saveTitles;
    for(const auto& element: titles) {
        saveTitles.push_back(element);
    }
    return saveTitles;
}
/**
 * We want to get an easy access to each vector.
 * This function help us to get the vector and save it by feature.
 * @return we returning the vector of the feature we want.
 */

vector<float> TimeSeries::vectorOfTitles(string feature) const{
    vector<float> saveVector;
    for(auto element: csvMap.find(feature)->second) {
        saveVector.push_back(element);
    }
    return saveVector;
}