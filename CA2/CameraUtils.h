#pragma once

#include "GameUtils.h"
#include "GameEngine.h"
#include "Fail.h"
#include "SpriteUtils.h"
#include "FontUtils.h"
#include "XMesh.h"
#include "ConsoleOutput.h"
#include <sstream>
#include "SceneEngine.h"
#include "MessageBoxScene.h"
#include "Node.h"
#include "Terrain.h" //Terrain
#include "ParticleSystem.h" //Particle System
#include "GamepadComponent.h" //Gamepad component
#include "Player.h" //Player
#include "Enemy.h" //Enemy

class CameraUtils : public CCameraNode
{
public:
	int mMode;
public:
	void TrackingCamera(CNode& cam, D3DXVECTOR3 pos,
		D3DXVECTOR3 tgt);

	void FirstPersonCamera(CNode& cam, CNode& tgt);

	void ThirdPersonCamera(CNode& cam, CNode& tgt);

	void OverheadFixedCamera(CNode& cam, CNode& tgt);

	void ChangeCameraMode(CCameraNode& cam, CNode& tgt);

	void InterpolateNode(CNode& cam, D3DXVECTOR3 tgt, float delta);
};