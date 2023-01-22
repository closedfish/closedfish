#pragma once

#include <chrono>
#include <thread>
#include <Windows.h>
#include <iostream>
#include "ChessBotUI.h"

//wrapper for the timer class
struct timer
{
	int delay = 10;
	bool shutdown = false;
};

namespace timers 
{
	void start_timer(timer& _timer, void (*func)(HWND), HWND param, int delay_val)
	{
		_timer.delay = delay_val;
		std::thread t([_timer, func, param]() {
			while (true)
			{
				if (_timer.shutdown == true)
					break;
				std::this_thread::sleep_for(std::chrono::milliseconds(_timer.delay));
				if (_timer.shutdown == true)
					break;
				func(param);
			}
		});
		t.detach();
	}

	void moveDetection(timer& _timer, bmpClass* bm)
	{
		std::thread t([_timer, bm]() {
			while (true)
			{
				if (_timer.shutdown == true)
					break;
				std::this_thread::sleep_for(std::chrono::milliseconds(_timer.delay));
				if (_timer.shutdown == true)
					break;
				while (!bm->canILookForPieces());
				bool move = bm->detectMove();
				if (move == true)
				{
					std::cout << "S-a intamplat nebunia\n";
					//Algo runs
				}
			}
		});
		t.detach();
	}
}	
