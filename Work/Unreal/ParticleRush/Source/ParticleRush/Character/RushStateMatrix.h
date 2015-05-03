// Particle Rush 2014 - 2015

#pragma once

#include "RushState.h"

#pragma region Layer Variables
static uint32_t LayerStartIndices[]	=		{ ERushState::Air };
static uint32_t LayerEndIndices[]	=		{ ERushState::Walk };
#pragma endregion

#pragma region State Matrices
static bool LayerMatrix[1][6][6] = 
{
	{ //Air		Boost	Bounce		HardStop	SharpTurn		Walk
	  {	true,	true,	true,		true,		true,			true}, // Air
	  {	true,	true,	true,		true,		true,			true}, // Boost
	  {	true,	true,	true,		true,		true,			true}, // Bounce
	  {	true,	true,	true,		true,		true,			true}, // HardStop
	  {	true,	true,	true,		true,		true,			true}, // SharpTurn
	  {	true,	true,	true,		true,		true,			true}  // Walk
	}
};
#pragma endregion