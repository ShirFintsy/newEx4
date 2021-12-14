cmake_minimum_required(VERSION 3.21)
project(ex4Temp)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex4Temp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h HybridAnomalyDetector.cpp
        HybridAnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h
        minCircle.cpp minCircle.h MainTrain.cpp)
