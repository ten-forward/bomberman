#ifndef INPUTSTATE_HPP
#define INPUTSTATE_HPP

struct JoystickState
{
    bool up : 1;
    bool down : 1;
    bool left : 1;
    bool right : 1;
	
    bool a : 1;
    bool b : 1;
    bool x : 1;
    bool y : 1;

	bool l1 : 1;
	bool l2 : 1;
	bool r1 : 1;
	bool r2 : 1;
};

class InputState
{	
    bool up : 1;
    bool down : 1;
    bool left : 1;
    bool right : 1;
	
    bool a : 1;
    bool b : 1;
    bool x : 1;
    bool y : 1;

	bool fingered;
	float fingerx;
	float fingery;

public:
	InputState();
	bool GetUpButtonState() const;
	bool GetDownButtonState() const;
	bool GetLeftButtonState() const;
	bool GetRightButtonState() const;

	bool GetAButtonState() const;
	bool GetBButtonState() const;
	bool GetXButtonState() const;
	bool GetYButtonState() const;
	
	bool GetFingered() const;
	float GetFingerX() const;
	float GetFingerY() const;

	void SetUpButtonState(bool state);
	void SetDownButtonState(bool state);
	void SetLeftButtonState(bool state);
	void SetRightButtonState(bool state);

	void SetFingered(bool state);
	void SetFingerX(float fx);
	void SetFingerY(float fy);

	void SetAButtonState(bool state);
	void SetBButtonState(bool state);
	void SetXButtonState(bool state);
	void SetYButtonState(bool state);
};

#endif // INPUTSTATE_HPP
