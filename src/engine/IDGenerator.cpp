#include "IDGenerator.h"
IDGenerator* IDGenerator::instance;
IDGenerator::IDGenerator() {
    IDGenerator::instance = this;
}
IDGenerator::~IDGenerator() {}

int IDGenerator::next() {
    id++;
    return id;
}