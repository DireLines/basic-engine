#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer {
public:
    /* Singleton pattern */
    static GameTimer* instance;

    GameTimer();
    ~GameTimer();

    static double Time() {
        return time;
    }
    static double DeltaTime() {
        return deltaTime;
    }

private:
    static double time;
    static double deltaTime;
    friend class Game;
};
#endif