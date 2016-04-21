#ifndef GAMETIME_H
#define GAMETIME_H

#include <QElapsedTimer>

class GameTime
{
private:
    static QElapsedTimer    timer;     //  Track elapsed time

    static float time;
    static float deltatime;

public:
    static Start()
    {
        timer.start();
        time = 0;
        deltatime = 0;
    }

    static void Tick()
    {
        float newtime = timer.restart() / 1000.f;
        deltatime = newtime - time;
        time += newtime;
    }

    static float GetTime() { return time; }
    static float GetDeltaTime() { return deltatime; }
};

#endif // GAMETIME_H
