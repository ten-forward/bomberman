#include "inputstate.hpp"
#include <cstring>

InputState::InputState()
:	
	fingered(false),
	fingerx(0),
	fingery(0)
{
	memset(down, 0, 32);
	memset(pressed, 0, 32);
	memset(released, 0, 32);
}

bool InputState::GetFingered() const
{
	return fingered;
}

float InputState::GetFingerX() const
{
	return fingerx;
}

float InputState::GetFingerY() const
{
	return fingery;
}

void InputState::SetFingered(bool state)
{
	fingered = state;
}

void InputState::SetFingerX(float fx)
{
	fingerx = fx;
}

void InputState::SetFingerY(float fy)
{
	fingery = fy;
}

