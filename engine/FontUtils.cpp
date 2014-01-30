/*==============================================
 * font functions for directX
 *
 * Written by <YOUR NAME HERE>
 *==============================================*/
#ifndef _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_DEPRECATE // remove a VS2005 warning
#endif
#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS // remove a VS2005 warning
#endif
#include "FontUtils.h"
#include "Fail.h"

ID3DXFont* CreateD3DFont(LPDIRECT3DDEVICE9 pDevice,const char* name,int height,bool bold)
{
	ID3DXFont* font=0;
	
	int weight = (bold) ? FW_BOLD : FW_NORMAL;

	D3DXCreateFontA(pDevice, height, 0, weight, 0, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, name, &font);

	return font;
}

 void DrawD3DFont(ID3DXFont* pFont,const char* text,int x,int y,D3DCOLOR col)
{
	 RECT r;
	 r.left = x, r.right = 2000, r.top = y, r.bottom = 2000;
	 pFont->DrawTextA(NULL, text, -1, &r, DT_TOP | DT_LEFT, col);
}

void DrawD3DFont(ID3DXFont* pFont,const std::string& text,int x,int y,D3DCOLOR col)
{
	DrawD3DFont(pFont,text.c_str(),x,y,col);
}

void DrawD3DFontEx(ID3DXFont* pFont,LPD3DXSPRITE pSpriteBat,const char* text,int x,int y,D3DCOLOR col,DWORD flags)
{
	RECT r={x,y,x,y};
	pFont->DrawText(pSpriteBat,text,-1,&r,flags,col);
}

void DrawD3DFontEx(ID3DXFont* pFont,LPD3DXSPRITE pSpriteBat,
				   const char* text,const D3DXVECTOR2& pos,
				   D3DCOLOR col,const D3DXVECTOR2& origin,
				   float scale,float rotate)
{
	RECT r={0,0,0,0};

	// we set up it all using matrixes (yes its complex)
	D3DXMATRIX orig,sc,tr,rot;
	D3DXMatrixTranslation(&orig,-origin.x,-origin.y,0);	// offset for text centre
	D3DXMatrixScaling(&sc,scale,scale,0);	// scale
	D3DXMatrixRotationZ(&rot,rotate);	// rotation
	D3DXMatrixTranslation(&tr,pos.x,pos.y,0);	// translation

	D3DXMATRIX res=orig*sc*rot*tr;
	pSpriteBat->SetTransform(&res);
	pFont->DrawText(pSpriteBat,text,-1,&r,DT_TOP|DT_LEFT|DT_NOCLIP,col);
	pSpriteBat->SetTransform(D3DXMatrixIdentity(&res));	// reset the matrix
}

D3DXVECTOR2 GetTextSize(ID3DXFont* pFont,const char* text)
{
	RECT r={0,0,0,0};
	pFont->DrawText(NULL,text,-1,&r,DT_TOP| DT_LEFT|DT_CALCRECT,0);
	return D3DXVECTOR2((float)r.right,(float)r.bottom);
}
