#ifndef INPUTSTATE_HPP
#define INPUTSTATE_HPP

class InputState
{
    bool up : 1;
    bool down : 1;
    bool left : 1;
    bool right : 1;
	
    bool a : 1;
    bool b : 1;
    bool c : 1;
    bool d : 1;
public:
	InputState();
	bool GetUpButtonState() const;
	bool GetDownButtonState() const;
	bool GetLeftButtonState() const;
	bool GetRightButtonState() const;
	
	void SetUpButtonState(bool state);
	void SetDownButtonState(bool state);
	void SetLeftButtonState(bool state);
	void SetRightButtonState(bool state);
};

#endif // INPUTSTATE_HPP
