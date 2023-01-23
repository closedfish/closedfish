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

	void moveDetection(timer& _timer, bmpClass* bm, HWND activeConsole)
	{
		std::thread t([_timer, bm, activeConsole]() {
			while (true)
			{
				if (_timer.shutdown == true)
					break;
				std::this_thread::sleep_for(std::chrono::milliseconds(_timer.delay));
				if (_timer.shutdown == true)
					break;
				while (!bm->canILookForPieces());
				bool move = bm->detectMove();

				Sleep(100);
				if (move == true)
				{

					//Algo runs
					string fen = bm->_returnFen();
					std::ofstream fout("Debug\\fen.txt");

					fout << fen;

					fout.close();

					Sleep(200);
				
					// Press the Alt and Tab key
					keybd_event(VK_MENU, 0x38, 0, 0);
					keybd_event(VK_TAB, 0x0F, 0, 0);

					Sleep(200);

					// Release the Tab and Alt key
					keybd_event(VK_TAB, 0x0F, KEYEVENTF_KEYUP, 0);
					keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

					Sleep(200);

					keybd_event(VK_NUMPAD9, 0, 0, 0); // press the '9' key
					Sleep(50);
					keybd_event(VK_NUMPAD9, 0, KEYEVENTF_KEYUP, 0); // release the '9' key
					Sleep(50);
					keybd_event(VK_RETURN, 0, 0, 0); // press the enter key
					Sleep(50);
					keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0); // release the enter key

					Sleep(200);
					//MessageBox(NULL, (LPCTSTR)fen.c_str(), TEXT("fen"), MB_OK);
					/*for (int i = 0; i < fen.length(); ++i)
					{
						if (fen[i] == '/')
						{
							keybd_event(VK_DIVIDE, 0x35, 0, 0);
							Sleep(10);
							keybd_event(VK_DIVIDE, 0x35, KEYEVENTF_KEYUP, 0);
							Sleep(40);
						}
						if (fen[i] = ' ')
						{
							keybd_event(VK_SPACE, 0x39, 0, 0);
							Sleep(10);
							keybd_event(VK_SPACE, 0x39, KEYEVENTF_KEYUP, 0);
							Sleep(40);
						}
						if (('a' <= fen[i] && 'z' >= fen[i]) || ('0' <= fen[i] && '9' >= fen[i]))
						{
							keybd_event((BYTE)fen[i], 0, 0, NULL);
							Sleep(40);
							keybd_event((BYTE)fen[i], 0, KEYEVENTF_KEYUP, NULL);
							Sleep(40);
						}

						if ('A' <= fen[i] && 'Z' >= fen[i])
						{

							keybd_event(VkKeyScan(fen[i]), 0, 0, NULL);
							Sleep(40);
							keybd_event(VkKeyScan(fen[i]), 0, KEYEVENTF_KEYUP, NULL);
							Sleep(40);
						}*/
						/*switch (fen[i])
						{
							case '/':
								break;
							case ' ':
							default:
								keybd_event((BYTE)fen[i], MapVirtualKey(fen[i], 0), 0, NULL);
								Sleep(10);
								keybd_event((BYTE)fen[i], MapVirtualKey(fen[i], 0), KEYEVENTF_KEYUP, NULL);
								/*if (('a' <= fen[i] && 'z' >= fen[i]))
								{
									keybd_event((BYTE)fen[i], MapVirtualKey(fen[i], 0), 0, NULL);
									Sleep(10);
									keybd_event((BYTE)fen[i], MapVirtualKey(fen[i], 0), KEYEVENTF_KEYUP, NULL);
								}
								if (('0' <= fen[i] && '9' >= fen[i]))
								{
									keybd_event((BYTE)fen[i], MapVirtualKey(fen[i], 0), 0, NULL);
									Sleep(10);
									keybd_event((BYTE)fen[i], MapVirtualKey(fen[i], 0), KEYEVENTF_KEYUP, NULL);

								}
								else
								{
									keybd_event((BYTE)(fen[i] + 32), NULL, 0, NULL);
									keybd_event(VK_SHIFT, 0x2A, 0, 0);
									Sleep(50);
									keybd_event((BYTE)(fen[i] + 32), NULL, KEYEVENTF_KEYUP, NULL);
									keybd_event(VK_SHIFT, 0x2A, KEYEVENTF_KEYUP, 0);
								}
								break;
						}*/
					Sleep(50);
					keybd_event(VK_RETURN, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
					keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
					Sleep(200);
					// Press the Alt and Tab key
					keybd_event(VK_MENU, 0x38, 0, 0);
					keybd_event(VK_TAB, 0x0F, 0, 0);

					Sleep(200);

					// Release the Tab and Alt key
					keybd_event(VK_TAB, 0x0F, KEYEVENTF_KEYUP, 0);
					keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

					Sleep(20000);

					}
				
			}
		});
		t.detach();
	}
}	
