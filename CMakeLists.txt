cmake_minimum_required(VERSION 3.17)
project(untitled9)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 14)

add_executable(untitled9 MainST.cpp anomaly_detection_util.cpp Server.cpp
        SimpleAnomalyDetector.cpp timeseries.cpp HybridAnomalyDetector.cpp minCircle.cpp CLI.cpp)