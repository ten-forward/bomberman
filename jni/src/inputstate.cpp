#include "inputstate.hpp"

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
