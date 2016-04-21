#ifndef GAMETIME_H
#define GAMETIME_H

#include <QElapsedTimer>

class GameTime
{
private:
    static QElapsedTimer timer;     //  Track elapsed time

    static float time;
    static float deltatime;

    static bool running;

public:

    static void Start();

    static void Tick();

    static float Time();
    static float DeltaTime();
};

#endif // GAMETIME_H
