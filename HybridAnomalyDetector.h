

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	explicit HybridAnomalyDetector(float threshold);
    virtual void usual(const TimeSeries& ts, correlatedFeatures sf, float maxP, int size,
                       Point** pointArr);
    virtual float checkDist(correlatedFeatures j, Point *c);
    //virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    /*virtual void learnNormal(const TimeSeries& ts);
    virtual void circleCorrelation(const TimeSeries& ts, correlatedFeatures sf, int size,
                                   Point** pointArr, float *x, float *y);*/
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
