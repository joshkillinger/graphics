#include "gametime.h"
#include <iostream>

using namespace std;

QElapsedTimer GameTime::timer;     //  Track elapsed time

float GameTime::time = 0;
float GameTime::deltatime = 0;

bool GameTime::running = false;

void GameTime::Start()
{
    if (GameTime::running)
    {
        cerr << "GameTime is already running!" << endl;
        return;
    }

    GameTime::timer.start();
    GameTime::time = 0;
    GameTime::deltatime = 0;

    GameTime::running = true;

    cout << "GameTime started" << endl;
}

void GameTime::Tick()
{
    GameTime::deltatime = GameTime::timer.restart() / 1000.0;
    GameTime::time += GameTime::deltatime;

    //cout << "GameTime time " << time << " deltatime " << deltatime << endl;
}

float GameTime::Time() { return GameTime::time; }
float GameTime::DeltaTime() { return GameTime::deltatime; }
