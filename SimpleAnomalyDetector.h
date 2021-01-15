#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

//#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Point *circleCor;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
    float thresholdMin = 0.9;
public:
    explicit SimpleAnomalyDetector(float thresholdMin);
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void usual(const TimeSeries& ts, correlatedFeatures sf, float maxP, int size,
                       Point** pointArr);
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    void setNormalModel(vector<correlatedFeatures> sf){
        cf = sf;
    }
    virtual float checkDist(correlatedFeatures j, Point *c);

    float getThresholdMin(){
        return this->thresholdMin;
    }
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
