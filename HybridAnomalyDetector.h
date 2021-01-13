

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	virtual void learnNormal(const TimeSeries& ts);
    virtual void circleCorrelation(const TimeSeries& ts, correlatedFeatures sf, int size,
                                   Point** pointArr, float *x, float *y);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
