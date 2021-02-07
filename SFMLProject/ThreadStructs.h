#pragma once

class DinoManager;

struct dinoinit_args
{
	DinoManager* dinoManager;
	int amount;
	int iteration;
	dinoinit_args(DinoManager* _dinoMgr, int _amount, int _iteration) : dinoManager(_dinoMgr), amount(_amount), iteration(_iteration) {}
};