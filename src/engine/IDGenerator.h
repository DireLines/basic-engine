#ifndef IDGENERATOR_H
#define IDGENERATOR_H
using namespace std;

class IDGenerator {
public:
    /* Singleton pattern */
    static IDGenerator* instance;

    IDGenerator();
    ~IDGenerator();

    int next();
private:
    int id = -1;
};
#endif