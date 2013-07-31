#include "inputstate.hpp"

InputState::InputState()
{
	up = false;
	down = false;
	left = false;
	right = false;

	fingered = false;
	fingerx = 0;
	fingery = 0;
}


bool InputState::GetUpButtonState() const
{
	return up;
}

bool InputState::GetDownButtonState() const
{
	return down;
}

bool InputState::GetLeftButtonState() const
{
	return left;
}

bool InputState::GetRightButtonState() const
{
	return right;
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


void InputState::SetUpButtonState(bool state)
{
	up = state;
}

void InputState::SetDownButtonState(bool state)
{
	down = state;
}

void InputState::SetLeftButtonState(bool state)
{
	left = state;
}

void InputState::SetRightButtonState(bool state)
{
	right = state;
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
