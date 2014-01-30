/*==============================================
 * utility functions for GDEV Engine
 *
 * Written by <YOUR NAME HERE>
 *
 *==============================================*/

#include "GameUtils.h"
#include "Fail.h"


void CameraInit(LPDIRECT3DDEVICE9 pd3dDevice,float fov, float aspect,float nearDist,float farDist)
{
	D3DXMATRIX theProjectionMatrix;
	D3DXMatrixPerspectiveFovLH(&theProjectionMatrix,//the matrix that will be written to
		fov,//view angle or FOV
		aspect,//Aspect Ratio
		nearDist,//near plane
		farDist//far plane
		);

	pd3dDevice->SetTransform(D3DTS_PROJECTION, &theProjectionMatrix);
}

void CameraLookAt(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 pos,D3DXVECTOR3 tgt,D3DXVECTOR3 up)
{
	D3DXMATRIX theViewMatrix;

	D3DXMatrixLookAtLH(&theViewMatrix, &pos, &tgt, &up);
	pd3dDevice->SetTransform(D3DTS_VIEW, &theViewMatrix);
}

void SetupDefaultD3DOptions(LPDIRECT3DDEVICE9 pDev, bool lights) 
{
	CameraInit(pDev);

	pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDev->SetRenderState(D3DRS_LIGHTING, true);
	pDev->SetRenderState(D3DRS_SPECULARENABLE, true);
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(20, 20, 20));

	D3DLIGHT9 light = InitDirectionalLight(D3DXVECTOR3(5, -10, 4.9f), D3DCOLOR_XRGB(255, 255, 255));
	
	pDev->SetLight(0, &light);
	pDev->LightEnable(0, lights);
}


D3DLIGHT9 InitDirectionalLight(const D3DXVECTOR3& direction, const D3DXCOLOR& colour)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	D3DXVECTOR3 inDirection = direction;

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = colour;
	light.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	D3DXVec3Normalize(&inDirection, &direction);
	light.Direction = direction;

	return light;
}

D3DLIGHT9 InitPointLight(const D3DXVECTOR3& position,const D3DXCOLOR& colour)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Diffuse = colour;
	light.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	light.Position = position;
	light.Range = 9.0f;

	return light;
}

D3DLIGHT9 InitSpotLight(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const D3DXCOLOR& color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = color;
	light.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 255, 255));
	light.Position = position;
	light.Direction = direction;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = D2R(30);
	light.Phi = D2R(45);
	light.Range = 9.0f;
	light.Falloff = 1;

	return light;
}

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

///////////////////////////////////////////////////////////////////////
#include "GameWindow.h"

D3DXVECTOR3 GetKeyboardVector(const int keys[6])
{
	D3DXVECTOR3 result(0,0,0);
	if (keys[0]>0 && CGameWindow::KeyDown(keys[0]))
		result.x--;
	if (keys[1]>0 && CGameWindow::KeyDown(keys[1]))
		result.x++;
	if (keys[2]>0 && CGameWindow::KeyDown(keys[2]))
		result.y--;
	if (keys[3]>0 && CGameWindow::KeyDown(keys[3]))
		result.y++;
	if (keys[4]>0 && CGameWindow::KeyDown(keys[4]))
		result.z--;
	if (keys[5]>0 && CGameWindow::KeyDown(keys[5]))
		result.z++;
	return result;
}

D3DXVECTOR3 GetMouseTurnVector(float sensitive,bool resetMouse)
{
	POINT p=CGameWindow::GetMouseDelta();
	if (resetMouse)	CGameWindow::SetMousePos(640, 360);
	D3DXVECTOR3 result(p.y/sensitive,p.x/sensitive,0);	// scale towards -1..1
	if (result.x<-1)	result.x=-1;
	if (result.x>+1)	result.x=+1;
	if (result.y<-1)	result.y=-1;
	if (result.y>+1)	result.y=+1;
	return result;
}


void ResetWorldMatrix(LPDIRECT3DDEVICE9 pDev)
{
	pDev->SetTransform(D3DTS_WORLD,&IDENTITY_MAT);
}

//////////////////////////////////////////////////////////////
float randf()
{
	return (float)(rand())/(RAND_MAX-1);
}

float randf(float a,float b)
{
	return ((float)(rand())/(RAND_MAX-1)*(b-a))+a;
}
int randi(int n)
{
	return rand()%n;
}

int randi(int a,int b)
{
	return (rand()%(b-a+1))+a;
}