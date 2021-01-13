#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

class TimeSeries{
    std::map<string, vector<float>> csvMap;
    vector<string> titles;
public:
    TimeSeries(const char* CSVfileName);
    vector<string> getTitles() const;
    vector<float> vectorOfTitles(string feature) const;
    float getFeature(string feature, float time);
    void addRow(vector<float>);
};



#endif /* TIMESERIES_H_ */