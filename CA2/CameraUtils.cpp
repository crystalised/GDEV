#include "CameraUtils.h"

void CameraUtils::TrackingCamera(CNode& cam, D3DXVECTOR3 pos,
	D3DXVECTOR3 tgt)
{
	InterpolateNode(cam, pos, 0.01f);
	cam.SetPos(cam.GetPos());
	cam.LookAt(tgt);
}

void CameraUtils::FirstPersonCamera(CNode& cam, CNode& tgt)
{
	cam.SetPos((tgt.GetPos() + D3DXVECTOR3(0, 0.5f, 0)));
	cam.SetHpr(tgt.GetHpr());
}

void CameraUtils::ThirdPersonCamera(CNode& cam, CNode& tgt)
{
	D3DXVECTOR3 offset = tgt.OffsetPos(D3DXVECTOR3(0, 1.5, -5));
	InterpolateNode(cam, offset, 0.10f);
	cam.SetPos(cam.GetPos());
	cam.LookAt(tgt);
}

void CameraUtils::OverheadFixedCamera(CNode& cam, CNode& tgt)
{
	D3DXVECTOR3 offset = D3DXVECTOR3(0, 10, -0.1f);
	InterpolateNode(cam, offset, 0.01f);
	cam.SetPos(cam.GetPos());
	cam.LookAt(tgt);
}

void CameraUtils::ChangeCameraMode(CCameraNode& cam, CNode& tgt)
{
	if (CGameWindow::KeyPress(VK_F1))		mMode = 0;
	if (CGameWindow::KeyPress(VK_F2))		mMode = 1;
	if (CGameWindow::KeyPress(VK_F3))		mMode = 2;
	if (CGameWindow::KeyPress(VK_F4))		mMode = 3;
	if (CGameWindow::KeyPress(VK_F5))		mMode = 4;

	switch (mMode)
	{
	case 0:	// tracking
		//TODO("set the camera's location");
		//TODO("make the camera look at the player");
		CameraUtils::TrackingCamera(cam, D3DXVECTOR3(0, 100, -5), tgt.GetPos());
		break;
	case 1: // first person
		cam.SetNearFar(0.1f, 10000.0f);
		//TODO("set pos & hpr to match player");
		CameraUtils::FirstPersonCamera(cam, tgt);
		break;
	case 2: // plank third person
		//TODO("Set the camera's position to be behind the player (5 units) & slightly up (3 units)");
		//TODO("make the camera look at the player");
		CameraUtils::ThirdPersonCamera(cam, tgt);
		break; // fixed overhead
	case 3:
		//TODO("Set the camera's position to be above the player");
		//TODO("make the camera look at the player");
		CameraUtils::OverheadFixedCamera(cam, tgt);
		break;
	default:	// fixed:
		//GetEngine()->SetTitle("Fixed Camera");
		InterpolateNode(cam, D3DXVECTOR3(5, 20, 4.9f), 0.10f);
		cam.SetPos(cam.GetPos());
		cam.LookAt(D3DXVECTOR3(5, 0, 5));
		break;
	}
}

void CameraUtils::InterpolateNode(CNode& cam, D3DXVECTOR3 tgt, float delta)
{
	D3DXVECTOR3 newPos;
	D3DXVec3Lerp(&newPos, &cam.GetPos(), &tgt, delta);
	cam.mPos = newPos;
}