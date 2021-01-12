
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}
/**
 * We want to have a title vector that can help us run all the vectors in the ts, vector by vector.
 * Helps us to reach each of the vectors like an array.
 * @param ts the time series we receive
 * @param t the string of our features
 * @return a vector that contain all the vector of the ts.
 */
vector<vector<float>> titleVector(const TimeSeries& ts , vector<string> t) {
    vector<vector<float>> v;
    for(int i = 0; i < ts.getTitles().size(); i++) {
        v.push_back(ts.vectorOfTitles(t[i]));
    }
    return v;
}
/**
 * This function learn which of the vector correlated to the others, and learns the correlated features.
 * @param ts the time series
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    vector<string> titles = ts.getTitles();
    vector<vector<float>> theVectors = titleVector(ts, titles);
    int size = theVectors[0].size();
    float* x;
    float* y;
    float maxD = 0, maxP = 0;
    Point** pointArr = new Point*[size];
    for(int i = 0; i < titles.size()- 1; i++) {
        x = const_cast<float *>(theVectors[i].data());
        for(int j = i + 1; j < titles.size(); j++) {
            correlatedFeatures sf;
            y = const_cast<float *>(theVectors[j].data());
            sf.feature1 = titles[i];
            sf.feature2 = titles[j];
            maxP = abs(pearson(x, y, size));
            if(maxP >= 0.9) {
                sf.corrlation = maxP;
                for(int k = 0; k < size; k++) {
                    pointArr[k] = new Point(x[k], y[k]);
                }
                sf.lin_reg = linear_reg(pointArr, size);
                for(int s = 0; s < size; s++) {
                     if(maxD < dev(*pointArr[s], sf.lin_reg)) {
                         maxD = dev(*pointArr[s], sf.lin_reg);
                     }
                }
                sf.threshold = maxD * 1.2;
                cf.push_back(sf);
            }
        }
    }
}
/**
 * This function receive the flights, row by row and checks if there is a point that too far away.
 * If there is a point like that we save the time of it and add it to our report.
 * @param ts the time series we receiving.
 * @return our report of points that not correlated enough.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    vector<string> titles = ts.getTitles();
    vector<AnomalyReport> report;
    vector<vector<float>> theVectors = titleVector(ts, titles);
    string saveReport;
    vector<float> one, two;
    for(int i = 0; i < theVectors[0].size(); i ++) {
        for(auto & j : cf) {
            one = ts.vectorOfTitles(j.feature1);
            two = ts.vectorOfTitles(j.feature2);
            auto *c = new Point(one[i], two[i]);
            if(dev(*c, j.lin_reg) > j.threshold) {
                saveReport = j.feature1 + "-" + j.feature2;
                AnomalyReport r = AnomalyReport(saveReport, i + 1);
                report.push_back(r);
            }
        }
    }
    return report;
}

