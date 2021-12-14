/*
 * Assignment 2
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */
#include "SimpleAnomalyDetector.h"


SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

/**
 * This function responsible about the learn part (learn the normal)
 * @param ts - time series with the normal info
 */


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    /*
     * find the correlated features in the time series- checking every couple of features.
     */
    for (int i = 0; i < ts.get_num_columns(); ++i) {
        float m = 0.9, h = 0.5;
        float pear;
        float *arrayI = from_vector_to_array(ts.get_column_by_loc(i)); // create array from info in feature 1
        for (int j = i + 1; j < ts.get_num_columns(); ++j) {
            float *arrayJ = from_vector_to_array(ts.get_column_by_loc(j)); // create array from info in feature 2
            float negPear = pearson(arrayI, arrayJ, ts.get_column_by_loc(j).size());
            pear = abs(negPear); // find the pearson of the features togther
            if (pear >= m) {
                m = pear;
                create_cor_feature_sim(pear, i, j, ts);
            } else if (negPear > h && negPear < m) {
                h = pear;
                correlatedFeatures c = create_cor_feature_cir(pear, i, j, ts);
                cf.push_back(c);
            }
        }
    }
}

void SimpleAnomalyDetector::create_cor_feature_sim(float pear, int feature1, int feature2,const TimeSeries &ts){
    struct correlatedFeatures cor;
    cor.feature1 = ts.get_head_line_by_loc(feature1);
    cor.feature2 = ts.get_head_line_by_loc(feature2);

    cor.corrlation = pear;
    cor.linReg = find_linear_reg(ts, cor.feature1, cor.feature2);
    Point** p = find_points_of_correlated_features(ts, cor.feature1, cor.feature2);
    cor.threshold = max_threshold(p, cor.linReg, ts.get_column_by_head(cor.feature1).size());
    //cor.line = return_timeStep(p, cor.linReg,ts.get_column_by_head(cor.feature1).size());
    cor.isCircleDetect = false;
    cf.push_back(cor);
}

/**
 * This function return the linear reg of the points of the correlated features.
 * @param c - the correlatedFeatures contain the info about the features
 * @param ts - time series
 * @return line return from linear_reg
 */

Line find_linear_reg (const TimeSeries &ts, string feat1, string feat2) {
    int sizeOfFeature = ts.get_column_by_head(feat1).size();
    Point** pointsArray = find_points_of_correlated_features(ts, feat1, feat2);
    Line l = linear_reg(pointsArray, sizeOfFeature);
    return l;
}
/**
 * This function convert vector to array (so we can use it in "pearson")
 * @param feature - the vector that contains the feature info
 * @return array
 */
float* from_vector_to_array(vector<float> feature) {
    float* array = new float[feature.size()];
    for (int i = 0; i < feature.size();i++) {
        array[i] = feature[i];
    }
    return array;
}
/**
 * This function create the points from the info in the correlated features.
 * @param c  - the correlatedFeatures contain the info about the features
 * @param ts - time series
 * @return array of pointer to points
 */
Point** find_points_of_correlated_features (const TimeSeries &ts, string feat1, string feat2) {
    int sizeOfFeature = ts.get_column_by_head(feat1).size();
    Point** pointsArray = new Point*[sizeOfFeature];
    for (int i = 0; i < sizeOfFeature; i++) {
        // create point from line i in feature 1 and line i in feature 2.
        Point *p = new Point(ts.get_column_by_head(feat1)[i],ts.get_column_by_head(feat2)[i]);
        pointsArray[i] = p;
    }
    return pointsArray;
}
/**
 * This function return the maximum threshold.
 * @param points - the points in the graph of the correlated features.
 * @param line - the linear reg of the graph.
 * @param size - the size of the points array.
 * @return maximum threshold value
 */
float max_threshold (Point** points, Line line, int size) {
    float maxThreshold = 0;
    for (int i = 0; i < size; ++i) {
        float d = dev(*points[i], line);
        if (d > maxThreshold) {
            maxThreshold = d;
        }
    }
    return 1.1 * maxThreshold;

}


/**
 * This function initilaied all fiels in correlatedFeatures class- to crean a new one.
 * @param cor - the object we want to create.
 * @param ts - time series
 * @param pear - the correlation
 * @param i - feature 1
 * @param c - feature 2
 */


/**
 * This function responsible about the test part (detect exceptions)
 * @param ts -time series
 * @return a vector contain all exceptions (AnomalyReport)
 */

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reports;
    for (const correlatedFeatures &i : this->cf) {
        if (i.isCircleDetect) { // the case of circle detect
            AnomalyReport s = check_if_inside(ts, i);
            if (s.timeStep != 0) {
                reports.push_back(s);
            }
        } else { // not a circle detector
            AnomalyReport s = check_if_detect(ts, i);
            if (s.timeStep != 0) {
                reports.push_back(s);
            }
        }
    }
    return reports;
}

AnomalyReport check_if_detect(const TimeSeries &ts, const correlatedFeatures &i) {
    int sizeOfFeature = ts.get_column_by_head(i.feature1).size();
    Line line = find_linear_reg(ts, i.feature1, i.feature2);
    for (int j = 0; j < sizeOfFeature; j++) {
        // create point from line i in feature 1 and line i in feature 2.
        Point p = Point(ts.get_column_by_head(i.feature1)[j], ts.get_column_by_head(i.feature2)[j]);

        if (i.threshold < dev(p, line)) {
            return AnomalyReport(i.feature1 + "-" + i.feature2, j + 1);
        }
    } return AnomalyReport("", 0);
}


correlatedFeatures SimpleAnomalyDetector::create_cor_feature_cir(float pear, int feature1, int feature2, const TimeSeries &ts) {
    correlatedFeatures c;
    return c;
}

AnomalyReport SimpleAnomalyDetector::check_if_inside(const TimeSeries &ts, const correlatedFeatures &i) {
    return AnomalyReport("", 0);
}
