#pragma once

class InputState
{	
    bool up : 1;
    bool down : 1;
    bool left : 1;
    bool right : 1;
	
    bool up_pressed : 1;
    bool down_pressed : 1;
    bool left_pressed : 1;
    bool right_pressed : 1;

    bool up_released : 1;
    bool down_released : 1;
    bool left_released : 1;
    bool right_released : 1;
	
    bool a : 1;
    bool b : 1;
    bool x : 1;
    bool y : 1;
    bool start : 1;
	
    bool a_pressed : 1;
    bool b_pressed : 1;
    bool x_pressed : 1;
    bool y_pressed : 1;
    bool start_pressed : 1;
	
    bool a_released : 1;
    bool b_released : 1;
    bool x_released : 1;
    bool y_released : 1;
    bool start_released : 1;

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
	bool GetStartButtonState() const;
	
	bool GetUpButtonJustPressed() const;
	bool GetDownButtonJustPressed() const;
	bool GetLeftButtonJustPressed() const;
	bool GetRightButtonJustPressed() const;

	bool GetAButtonJustPressed() const;
	bool GetBButtonJustPressed() const;
	bool GetXButtonJustPressed() const;
	bool GetYButtonJustPressed() const;
	bool GetStartButtonJustPressed() const;

	bool GetAButtonJustReleased() const;
	bool GetBButtonJustReleased() const;
	bool GetXButtonJustReleased() const;
	bool GetYButtonJustReleased() const;
	bool GetStartButtonJustReleased() const;

	bool GetUpButtonJustReleased() const;
	bool GetDownButtonJustReleased() const;
	bool GetLeftButtonJustReleased() const;
	bool GetRightButtonJustReleased() const;

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
	void SetStartButtonState(bool state);
	

	void SetUpButtonJustPressed(bool state);
	void SetDownButtonJustPressed(bool state);
	void SetLeftButtonJustPressed(bool state);
	void SetRightButtonJustPressed(bool state);

	void SetAButtonJustPressed(bool state);
	void SetBButtonJustPressed(bool state);
	void SetXButtonJustPressed(bool state);
	void SetYButtonJustPressed(bool state);
	void SetStartButtonJustPressed(bool state);

	void SetAButtonJustReleased(bool state);
	void SetBButtonJustReleased(bool state);
	void SetXButtonJustReleased(bool state);
	void SetYButtonJustReleased(bool state);
	void SetStartButtonJustReleased(bool state);

	void SetUpButtonJustReleased(bool state);
	void SetDownButtonJustReleased(bool state);
	void SetLeftButtonJustReleased(bool state);
	void SetRightButtonJustReleased(bool state);

};
