cmake_minimum_required(VERSION 3.0.0)
project(Cmake VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Cmake MainTrain.cpp commands.h CLI.cpp CLI.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h minCircle.h minCircle.cpp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
