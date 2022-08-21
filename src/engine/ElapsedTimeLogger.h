#ifndef ELAPSEDTIMELOGGER_H
#define ELAPSEDTIMELOGGER_H
#include <string>
#include <iostream>
#include "WallTime.h"
using namespace std;

class ElapsedTimeLogger {
public:
    ElapsedTimeLogger(string logname) {
        this->logname = logname;
        prev = get_wall_time();
    }
    void operator()(string message) {
        double now = get_wall_time();
        cout << logname << ": " << message << " took " << (now-prev)*1000000 << " micros" << endl;
        prev = now;
    }
private:
    std::string logname;
    double prev;
};
#endif