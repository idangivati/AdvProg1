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
    Circle *circleCor;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void usual(const TimeSeries& ts, correlatedFeatures sf, float maxD, int size,
                       Point** pointArr, float *x, float *y);
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    void setNormalModel(vector<correlatedFeatures> sf){
        cf = sf;
    }

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
