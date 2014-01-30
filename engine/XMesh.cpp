/*==============================================
 * Improved X Mesh loader for GDEV Engine
 *
 * First version by Mark Gossage
 *==============================================*/
#pragma once
#include <string>
#include "XMesh.h"
#include "Fail.h"

std::string ExtractPath(const std::string& path)
{
	std::string::size_type pos = path.find_last_of("\\");
	if (pos == std::string::npos)
		pos = path.find_last_of("/");
	if (pos == std::string::npos)
		return "";
	return path.substr(0, pos) + "/";
}

CXMesh::CXMesh(LPDIRECT3DDEVICE9 pDev,const char * name)
{
	mpDev=pDev;
	mpMesh=0;

	LoadMesh(name);
	TidyMesh();	///< optimisation & repairs
	ComputeCollisionInfo();	///< computes the bounding sphere
}

CXMesh::~CXMesh()
{
	if (mpMesh != NULL)
	{
		mpMesh->Release();
	}
}

LPDIRECT3DTEXTURE9 LoadTexture(LPDIRECT3DDEVICE9 pDev, const std::string& name)
{
	LPDIRECT3DTEXTURE9 pTex;

	D3DXCreateTextureFromFileA(pDev, name.c_str(), &pTex);

	return pTex;
}

bool CXMesh::LoadMesh(const char* name)
{
	std::string path = ExtractPath(name);
	ID3DXBuffer* materialBuffer;
	DWORD numMaterials;

	D3DXLoadMeshFromXA(name, 0, mpDev, NULL, &materialBuffer, NULL, &numMaterials, &mpMesh);

	D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)(materialBuffer->GetBufferPointer());

	for (unsigned i = 0; i < numMaterials; i++)
	{
		D3DMATERIAL9 mtrl = d3dxMaterials[i].MatD3D;
		mtrl.Ambient = mtrl.Diffuse;
		mMats.push_back(mtrl);
		if (d3dxMaterials[i].pTextureFilename == NULL)
		{
			mTextures.push_back(NULL);
		}
		else
		{
			std::string filename = path+d3dxMaterials[i].pTextureFilename;
			IDirect3DTexture9* pTex = LoadTexture(mpDev, filename);
			mTextures.push_back(pTex);
		}
	}
	materialBuffer->Release();
	return true;
}

void CXMesh::TidyMesh()	///< optimisation & repairs
{
	if (mpMesh==NULL)	return;	// sanity

	// Does the mesh have a D3DFVF_NORMAL in its vertex format?
	if( !( mpMesh->GetFVF( ) & D3DFVF_NORMAL ) )
	{
		// No, so clone a new mesh and add D3DFVF_NORMAL to its vertex format:
		ID3DXMesh* theTempMesh = NULL;
		mpMesh->CloneMeshFVF( D3DXMESH_MANAGED, 
								mpMesh->GetFVF( ) | D3DFVF_NORMAL, // add normals to FVF here
								mpDev, 
								&theTempMesh );
		// Compute the normals using the mesh; this will insert normals into the mesh.
		D3DXComputeNormals( theTempMesh, NULL );
		// Dispose of old mesh
		mpMesh->Release( );
		// Reassign our pointer to the new generated mesh with normals.
		mpMesh = theTempMesh;
	}
}

void CXMesh::ComputeCollisionInfo()	///< computes the bounding sphere
{
	if (mpMesh==NULL)	return;	// sanity
	D3DXVECTOR3* pVertices=NULL;

	if (FAILED(mpMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices)))
	{
		mRadius=1.0f;	// a guess?
		return;
	}

	mRadius=0;
	D3DXVECTOR3 centre;
	D3DXComputeBoundingSphere(pVertices,	// the vertices
						mpMesh->GetNumVertices(),	// number of them 
						D3DXGetFVFVertexSize(mpMesh->GetFVF()),	// the struct size
						&centre,	// centre
						&mRadius);	// radius
	mpMesh->UnlockVertexBuffer();
}

void CXMesh::Draw()
{
	for (unsigned i = 0; i<mMats.size(); i++)
	{
		mpDev->SetMaterial(&mMats[i]);
		mpDev->SetTexture(0, mTextures[i]);
		mpMesh->DrawSubset(i);
	}
}

void CXMesh::Draw(const D3DXMATRIX& mat)	// renders at matrix
{
	mpDev->SetTransform(D3DTS_WORLD, &mat);
	Draw();
}
void CXMesh::Draw(const D3DXVECTOR3& pos,float scale)	// at some position
{
	D3DXMATRIX world, trans, mscale;
	D3DXMatrixTranslation(&trans, (pos.x + 0.5f), (pos.y), (pos.z + 0.5f));
	D3DXMatrixScaling(&mscale, 1.0f, 1.0f, 1.0f);
	world = mscale*trans;
	Draw(world);
}
