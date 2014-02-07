/*==============================================
 * GamepadComponent class, handles Gamepads via XInput
 *
 * Written by <YOUR NAME>
 *==============================================*/
#pragma once

#include <windows.h>
#include <XInput.h> // XInput API
#include <d3dx9.h>
#include <vector>
#include "GameComponent.h"

#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.


struct CONTROLLER_STATE
{
    XINPUT_STATE state;
    bool bConnected;
};


	
typedef enum{ NONE, LEFT_X, LEFT_X_INV, LEFT_Y, LEFT_Y_INV, RIGHT_X, RIGHT_X_INV, RIGHT_Y, RIGHT_Y_INV} AXIS_ENUM;	// the axis State 'type'


class CGamepadComponent: public CGameComponent
{
public:
	/// returns true if a given gamepad is connected.
	/// if its a invalid gamepad id it will return false
	bool IsGamepadConnected(int gamepadID = 0);

	/// returns true if gamepad button is down now 
	/// this does not check vs the previous state
	/// pre: gamepadID is a valid gamepad ID (0-3)
	/// pre: buttonID is a valid XINPUT gamepad button ID
	/// See http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx
	/// for the list of valid device buttons.
	bool IsButtonDown(int gamepadID, int buttonId);
	/// returns true if mouse button is down now, implement this if you want button press events
	//bool ButtonPress(int buttId);
	bool IsButtonPressed(int gamepadID, int buttonId);
	bool IsButtonPressed2(int gamepadID, int buttonId);
	bool IsButtonPressed3(int gamepadID, int buttonId);
	bool CheckState(int gamepadID, int buttonId);

	/// Calls GetAxis() to make a Vector3
	D3DXVECTOR3 GetVector(int gamepadID, AXIS_ENUM axisX, AXIS_ENUM axisY, AXIS_ENUM axisZ);

	/// helper which returns the axis value (0 if not valid/present)
	float GetAxis(int gamepadID, AXIS_ENUM axis);
	/// sets the vibration on left and right motors
	void SetVibration( int gamepadID, float leftMotor, float rightMotor );

	/// returns 0.0-1.0 as a percentage value of each trigger pressed
	float GetLeftTrigger( int gamepadID );
	float GetRightTrigger( int gamepadID );

	CGamepadComponent();	///< constructor
	void Startup();			///< set up everything
	void Update(float dt);	///< must be called once/cycle
	void Shutdown();		///< cleanup and releases memory

protected: // data
	CONTROLLER_STATE g_Controllers[MAX_CONTROLLERS];
	bool    g_bDeadZoneOn; // turn on Dead Zones by default
	bool buttonDown2;
	bool buttonDown3;
	bool buttonPrevState;
	bool buttonCurrState;
	float previousStick;
	DWORD previousState;
	DWORD currentState;
};
