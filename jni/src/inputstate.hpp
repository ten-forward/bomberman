#pragma once

class InputState
{	
	char down[32];
	char pressed[32];
	char released[32];

	bool fingered;
	float fingerx;
	float fingery;

	static bool GetBit(const char bitIndex, const char* array)
	{
		return (array[bitIndex >> 3] & ( 1 << (bitIndex & 0x7) )) != 0;
	}

	static void SetBit(char bitIndex, char* array, bool state)
	{
		char* block = &array[bitIndex >> 3];
		char clear = ~( 1 << (bitIndex & 0x7) );
		char set = ( state << (bitIndex & 0x7) );
		*block &= clear;
		*block |= set;
	}

public:

	enum Key
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		A,
		B,
		X,
		Y,
		L1,
		L2,
		R1,
		R2,
		START
	};

	InputState();

	bool GetButtonState(Key key) const
	{
		return GetBit(key, down);
	}
	
	bool GetButtonPressed(Key key) const
	{
		return GetBit(key, pressed);
	}
	
	bool GetButtonReleased(Key key) const
	{
		return GetBit(key, released);
	}
	
	void SetButtonState(Key key, bool state)
	{
		SetBit(key, down, state);
	}
	
	void SetButtonPressed(Key key, bool state)
	{
		SetBit(key, pressed, state);
	}
	
	void SetButtonReleased(Key key, bool state)
	{
		SetBit(key, released, state);
	}

	bool GetFingered() const;
	float GetFingerX() const;
	float GetFingerY() const;

	void SetFingered(bool state);
	void SetFingerX(float fx);
	void SetFingerY(float fy);
};
