/*
 * Assignment 4
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */
#include "HybridAnomalyDetector.h"
#include "minCircle.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

AnomalyReport HybridAnomalyDetector::check_if_inside(const TimeSeries &ts, const correlatedFeatures &cor) {
    int sizeOfFeature = ts.get_column_by_head(cor.feature1).size();
    Circle cir = Circle(cor.minCircle.center, cor.threshold);
    for (int i = 0; i < sizeOfFeature; i++) {
        // create point from line cor in feature 1 and line cor in feature 2.
        Point p = Point(ts.get_column_by_head(cor.feature1)[i], ts.get_column_by_head(cor.feature2)[i]);

        if (!is_inside(cir, p)) {
            return AnomalyReport(cor.feature1 + "-" + cor.feature2, i + 1);
        }
    } return AnomalyReport("", 0);
}

correlatedFeatures HybridAnomalyDetector::create_cor_feature_cir(float pear, int feature1, int feature2, const TimeSeries &ts) {
    struct correlatedFeatures cor;
    if (feature1 != -1 && feature2 != -1) {
        cor.feature1 = ts.get_head_line_by_loc(feature1);
        cor.feature2 = ts.get_head_line_by_loc(feature2);
    }
    cor.corrlation = pear;
    Point** p = find_points_of_correlated_features(ts, cor.feature1, cor.feature2);
    cor.minCircle = findMinCircle(p, ts.get_column_by_head(cor.feature1).size());
    cor.threshold = cor.minCircle.radius * 1.1;
    //cor.line = 0;
    cor.isCircleDetect = true;

    return cor;
}

