#include "HybridAnomalyDetector.h"
#include "minCircle.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

vector<vector<float>> features(const TimeSeries& ts , vector<string> t) {
    vector<vector<float>> v;
    for(int i = 0; i < ts.getTitles().size(); i++) {
        v.push_back(ts.vectorOfTitles(t[i]));
    }
    return v;
}

void HybridAnomalyDetector::circleCorrelation(const TimeSeries& ts, correlatedFeatures sf, int size,
                                              Point** pointArr, float *x, float *y) {
    for(int k = 0; k < size; k++) {
        pointArr[k] = new Point(x[k], y[k]);
    }
    Circle minCircle = findMinCircle(pointArr, size);
    minCircle.radius *= 1.1;
    sf.threshold = minCircle.radius;
    sf.circleCor = &minCircle;
    vector<correlatedFeatures> cf = getNormalModel();
    cf.push_back(sf);
    setNormalModel(cf);
}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
    vector<string> titles = ts.getTitles();
    vector<vector<float>> theVectors = features(ts, titles);
    int size = theVectors[0].size();
    float *x;
    float *y;
    float maxD = 0, maxP = 0;
    Point **pointArr = new Point *[size];
    for (int i = 0; i < titles.size() - 1; i++) {
        x = const_cast<float *>(theVectors[i].data());
        for (int j = i + 1; j < titles.size(); j++) {
            correlatedFeatures sf;
            y = const_cast<float *>(theVectors[j].data());
            sf.feature1 = titles[i];
            sf.feature2 = titles[j];
            maxP = abs(pearson(x, y, size));
            if (maxP >= 0.9) {
                sf.corrlation = maxP;
                SimpleAnomalyDetector::usual(ts, sf, maxD, size, pointArr, x, y);
            }
                // we will now do a special hybrid phase according to the instructions.
            else if (maxP > 0.5) {
                sf.corrlation = maxP;
                circleCorrelation(ts, sf, size, pointArr, x, y);
            }
        }
    }
}

void detectInCircle(correlatedFeatures j, Point c, int i, string saveReport, vector<AnomalyReport> *report) {
    float dist = distacneBetween(j.circleCor->center, c);
    if (dist > j.threshold) {
        saveReport = j.feature1 + "-" + j.feature2;
        AnomalyReport r = AnomalyReport(saveReport, i + 1);
        report->push_back(r);
    }
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    vector<string> titles = ts.getTitles();
    vector<AnomalyReport> report;
    vector<vector<float>> theVectors = features(ts, titles);
    string saveReport;
    vector<correlatedFeatures> cf = getNormalModel();
    vector<float> one, two;
    for(int i = 0; i < theVectors[0].size(); i ++) {
        for(auto & j : cf) {
            one = ts.vectorOfTitles(j.feature1);
            two = ts.vectorOfTitles(j.feature2);
            auto *c = new Point(one[i], two[i]);
            if(j.circleCor != nullptr) {
                detectInCircle(j, *c, i, saveReport, &report);
            }
            else if(dev(*c, j.lin_reg) > j.threshold) {
                saveReport = j.feature1 + "-" + j.feature2;
                AnomalyReport r = AnomalyReport(saveReport, i + 1);
                report.push_back(r);
            }
        }
    }
    return report;
}