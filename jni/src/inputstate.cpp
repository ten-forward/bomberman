#include "inputstate.hpp"

InputState::InputState()
:	up(false),
	down(false),
	left(false),
	right(false),

	up_pressed(false),
	down_pressed(false),
	left_pressed(false),
	right_pressed(false),

	up_released(false),
	down_released(false),
	left_released(false),
	right_released(false),

	a(false),
	b(false),
	x(false),
	y(false),
	
	a_pressed(false),
	b_pressed(false),
	x_pressed(false),
	y_pressed(false),
	
	a_released(false),
	b_released(false),
	x_released(false),
	y_released(false),

	fingered(false),
	fingerx(0),
	fingery(0)
{}


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


bool InputState::GetUpButtonJustPressed() const
{
	return up_pressed;
}

bool InputState::GetDownButtonJustPressed() const
{
	return down_pressed;
}

bool InputState::GetLeftButtonJustPressed() const
{
	return left_pressed;
}

bool InputState::GetRightButtonJustPressed() const
{
	return right_pressed;
}


bool InputState::GetAButtonJustPressed() const
{
	return a_pressed;
}

bool InputState::GetBButtonJustPressed() const
{
	return b_pressed;
}

bool InputState::GetXButtonJustPressed() const
{
	return x_pressed;
}

bool InputState::GetYButtonJustPressed() const
{
	return y_pressed;
}


bool InputState::GetAButtonJustReleased() const
{
	return a_released;
}

bool InputState::GetBButtonJustReleased() const
{
	return b_released;
}

bool InputState::GetXButtonJustReleased() const
{
	return x_released;
}

bool InputState::GetYButtonJustReleased() const
{
	return y_released;
}


bool InputState::GetUpButtonJustReleased() const
{
	return up_released;
}

bool InputState::GetDownButtonJustReleased() const
{
	return down_released;
}

bool InputState::GetLeftButtonJustReleased() const
{
	return left_released;
}

bool InputState::GetRightButtonJustReleased() const
{
	return right_released;
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

bool InputState::GetAButtonState() const
{
	return a;
}

bool InputState::GetBButtonState() const
{
	return b;
}

bool InputState::GetXButtonState() const
{
	return x;
}

bool InputState::GetYButtonState() const
{
	return y;
}

void InputState::SetAButtonState(bool state)
{
	a = state;
}

void InputState::SetBButtonState(bool state)
{
	b = state;
}

void InputState::SetXButtonState(bool state)
{
	x = state;
}

void InputState::SetYButtonState(bool state)
{
	y = state;
}


void InputState::SetUpButtonJustPressed(bool state)
{
	up_pressed = state;
}

void InputState::SetDownButtonJustPressed(bool state)
{
	down_pressed = state;
}

void InputState::SetLeftButtonJustPressed(bool state)
{
	left_pressed = state;
}

void InputState::SetRightButtonJustPressed(bool state)
{
	right_released = state;
}


void InputState::SetAButtonJustPressed(bool state)
{
	a_pressed = state;
}

void InputState::SetBButtonJustPressed(bool state)
{
	b_pressed = state;
}

void InputState::SetXButtonJustPressed(bool state)
{
	x_pressed = state;
}

void InputState::SetYButtonJustPressed(bool state)
{
	y_pressed = state;
}


void InputState::SetAButtonJustReleased(bool state)
{
	a_released = state;
}

void InputState::SetBButtonJustReleased(bool state)
{
	b_released = state;
}

void InputState::SetXButtonJustReleased(bool state)
{
	x_released = state;
}

void InputState::SetYButtonJustReleased(bool state)
{
	y_released = state;
}


void InputState::SetUpButtonJustReleased(bool state)
{
	up_released = state;
}

void InputState::SetDownButtonJustReleased(bool state)
{
	down_released = state;
}

void InputState::SetLeftButtonJustReleased(bool state)
{
	left_released = state;
}

void InputState::SetRightButtonJustReleased(bool state)
{
	right_released = state;
}

