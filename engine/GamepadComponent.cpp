/*==============================================
 * GamepadComponent class, handles Gamepads via XInput
 *
 * Written by <YOUR NAME>
 *==============================================*/
#include "GamepadComponent.h"
#include "SceneEngine.h"
#include "Fail.h"

CGamepadComponent::CGamepadComponent() :
	g_bDeadZoneOn(true) // set deadzone to true by default
{
	// TODO: Init state
	//...
	ZeroMemory(g_Controllers, sizeof(CONTROLLER_STATE)* MAX_CONTROLLERS);
	buttonCurrState = false;
	buttonPrevState = false;
}

void CGamepadComponent::Startup()
{
}

void CGamepadComponent::Shutdown()
{
}

void CGamepadComponent::Update(float dt)
{
	// TODO: Update controller(s) states
	DWORD dwResult;

	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &g_Controllers[i].state);

		if (dwResult == ERROR_SUCCESS)
			g_Controllers[i].bConnected = true;
		else
			g_Controllers[i].bConnected = false;
	}

	previousState = XInputGetState(0, &g_Controllers[0].state);
}

bool CGamepadComponent::IsGamepadConnected(int gamepadID)
{
	// TODO: sanity check for valid gamepadID
		// return false;


	// Returns true if a controller is connected.
	return g_Controllers[gamepadID].bConnected;
}


/**
Pre: buttonId is valid XINPUT_GAMEPAD_XXX type
e.g. XINPUT_GAMEPAD_A XINPUT_GAMEPAD_RIGHT_SHOULDER etc
*/
bool CGamepadComponent::IsButtonDown(int gamepadID, int buttonId)
{
	if (g_Controllers[gamepadID].bConnected)
	{
		// TODO: Get button states and return true if 
		// the button is currently down
		// ...
		WORD wButtons = g_Controllers[gamepadID].state.Gamepad.wButtons;
		if (wButtons & buttonId)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}

bool CGamepadComponent::IsButtonPressed(int gamepadID, int buttonId)
{
	buttonPrevState = buttonCurrState;
	if (!IsButtonDown(gamepadID, buttonId))
		buttonCurrState = false;
	else
		buttonCurrState = true;
	if (buttonPrevState == false && buttonCurrState == true)
		return true;
	else
		return false;
}

bool CGamepadComponent::IsButtonPressed2(int gamepadID, int buttonId)
{
	if (!IsButtonDown(gamepadID, buttonId))
		buttonDown2 = false;

	if (IsButtonDown(gamepadID, buttonId) && buttonDown2 == false)
	{
		buttonDown2 = true;
		return true;
	}
	else
		return false;
}

bool CGamepadComponent::IsButtonPressed3(int gamepadID, int buttonId)
{
	if (!IsButtonDown(gamepadID, buttonId))
		buttonDown3 = false;

	if (IsButtonDown(gamepadID, buttonId) && buttonDown3 == false)
	{
		buttonDown3 = true;
		return true;
	}
	else
		return false;
}

D3DXVECTOR3 CGamepadComponent::GetVector(int gamepadID, AXIS_ENUM axisX, AXIS_ENUM axisY, AXIS_ENUM axisZ)
{

	// TODO: Call GetAxis to build the vector based on the input axis for X, Y and Z values
	// ...
	if (g_Controllers[gamepadID].bConnected)
	{
		D3DXVECTOR3 controllerAxis;
		controllerAxis = D3DXVECTOR3(GetAxis(gamepadID, axisX), GetAxis(gamepadID, axisY), GetAxis(gamepadID, axisZ));
		return controllerAxis;
	}
}

float CGamepadComponent::GetAxis(int gamepadID, AXIS_ENUM axis)
{
	if( g_Controllers[gamepadID].bConnected )
	{
		WORD wButtons = g_Controllers[gamepadID].state.Gamepad.wButtons;

		// TODO: Deadzone checks...
		if (g_bDeadZoneOn)
		{
			// Zero value if thumbsticks are within the dead zone 
			if ((g_Controllers[gamepadID].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
				g_Controllers[gamepadID].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
				(g_Controllers[gamepadID].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
				g_Controllers[gamepadID].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
			{
				g_Controllers[gamepadID].state.Gamepad.sThumbLX = 0;
				g_Controllers[gamepadID].state.Gamepad.sThumbLY = 0;
			}

			if ((g_Controllers[gamepadID].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
				g_Controllers[gamepadID].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
				(g_Controllers[gamepadID].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
				g_Controllers[gamepadID].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
			{
				g_Controllers[gamepadID].state.Gamepad.sThumbRX = 0;
				g_Controllers[gamepadID].state.Gamepad.sThumbRY = 0;
			}
		}
	}
	
	// TODO: Return -1 to 1 values for thumbstick x and y values, based on
	// maximum SHORT value
	switch ( axis )
	{
	case LEFT_X: 
		return g_Controllers[gamepadID].state.Gamepad.sThumbLX / 32768.0f;
		break;	
	case LEFT_X_INV:
		return -g_Controllers[gamepadID].state.Gamepad.sThumbLX / 32768.0f;
		break;
	case LEFT_Y:
		return g_Controllers[gamepadID].state.Gamepad.sThumbLY / 32768.0f;
		break;
	case LEFT_Y_INV:
		return -g_Controllers[gamepadID].state.Gamepad.sThumbLY / 32768.0f;
		break;
	case RIGHT_X:
		return g_Controllers[gamepadID].state.Gamepad.sThumbRX / 32768.0f;
		break;
	case RIGHT_X_INV:
		return -g_Controllers[gamepadID].state.Gamepad.sThumbRX / 32768.0f;
		break;
	case RIGHT_Y:
		return g_Controllers[gamepadID].state.Gamepad.sThumbRY / 32768.0f;
		break;
	case RIGHT_Y_INV:
		return -g_Controllers[gamepadID].state.Gamepad.sThumbRY / 32768.0f;
		break;
	}
	return 0;
}

void CGamepadComponent::SetVibration( int gamepadID, float leftMotor, float rightMotor )
{
	// TODO: Set the vibration rumble on each motor
	// ...
	if (g_Controllers[gamepadID].bConnected)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = leftMotor * 65535.0f; // use any value between 0-65535 here
		vibration.wRightMotorSpeed = rightMotor * 65535.0f; // use any value between 0-65535 here
		XInputSetState(gamepadID, &vibration);
	}

}

float CGamepadComponent::GetLeftTrigger( int gamepadID )
{
	// TODO: Query left trigger value based on max of 255
	// value returned should be between 0 to 1
	if (g_Controllers[gamepadID].bConnected)
	{
		return g_Controllers[gamepadID].state.Gamepad.bLeftTrigger / 255.0f;
	}
}

float CGamepadComponent::GetRightTrigger( int gamepadID )
{
		
	// TODO: Query right trigger value based on max of 255
	// value returned should be between 0 to 1
	if (g_Controllers[gamepadID].bConnected)
	{
		return g_Controllers[gamepadID].state.Gamepad.bRightTrigger / 255.0f;
	}
}

bool CGamepadComponent::CheckState(int gamepadID, int buttonId)
{
	if (previousState != currentState)
		return true;
	else
		return false;
	previousState = currentState;
	
}