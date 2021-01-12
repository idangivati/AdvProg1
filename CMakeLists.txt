cmake_minimum_required(VERSION 3.17)
project(untitled9)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled9 main.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp)