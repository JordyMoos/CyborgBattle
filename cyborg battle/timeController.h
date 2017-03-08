#pragma once

#include <iostream>
#include "SDL.h"

class TimeController
{
public:

	static const int TimeController::PLAY_STATE = 0;
	static const int TimeController::PAUSE_STATE = 1;

	int timeState;
	Uint32 lastUpdate; // Last tick
	float dT; // DeltaTime in seconds

	TimeController();

	void updateTime();

	void pause();

	void resume();

	void reset();

	static TimeController timeController;
};
