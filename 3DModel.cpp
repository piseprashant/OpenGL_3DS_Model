// 3DModel.cpp: implementation of the C3DModel class.
//
//////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "3dsFileFormat.h"
#include "3dsFileLoader.h"
#include "VECTOR.h"
#include "MATRIX.h"
#include "3DFace.h"
#include "3DCamera.h"
#include "3DObject.h"
#include "3DModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool C3DModel::NewObject()
{
	if (mStillOpen == true)
		return false;			// Previous session is still incomplete!
	else mStillOpen = true;

	if(mObjectsCount == NoObjects)
		mObjectsCount = 0;
	else mObjectsCount++;

	C3DObject	*objectPtr;
	objectPtr = new C3DObject;

	mObjects.push_back(objectPtr);

	return true;
}

bool C3DModel::EndObject()
{
	if (mStillOpen == false)
		return false;			// No active session
	else mStillOpen = false;
	return true;
}

bool C3DModel::AddVertex(float x, float y, float z)
{
	if (mStillOpen == false)
		return false;			// No active session

	mObjects[mObjectsCount]->vertices.push_back(VECTOR(x, y, z));

	return true;
}

bool C3DModel::AddFace(int A, int B, int C)
{
	if (mStillOpen == false)
		return false;			// No active session

	mObjects[mObjectsCount]->faces.push_back(C3DFace(A, B, C));

	return true;
}

bool C3DModel::AddTexturePoint(float u, float v)
{
	if (mStillOpen == false)
		return false;			// No active session

	mObjects[mObjectsCount]->texturePoints.push_back(VECTOR(u, v, 0));
	return true;
}

bool C3DModel::AddCamera(C3DCamera Camera)
{
	mCameras.push_back(Camera);
	return true;
}

bool C3DModel::SetMatrix(MATRIX Mtx)
{
	if (mStillOpen == false)
		return false;			// No active session

	mObjects[mObjectsCount]->transform = Mtx;
	return true;
}

bool C3DModel::SetTranslation(float x, float y, float z)
{
	if (mStillOpen == false)
		return false;			// No active session

	mObjects[mObjectsCount]->translate[0] = x;
	mObjects[mObjectsCount]->translate[1] = y;
	mObjects[mObjectsCount]->translate[2] = z;
	return true;
}

void C3DModel::User3dFace(unsigned short A, unsigned short B, unsigned short C, 
						  unsigned short Flags)
{
	AddFace((int) A, (int) B, (int) C);

	cout << "A: " << A << " B: " << B << " C:" << C << endl;
	cout << " Flags: " << Flags << endl;
}

void C3DModel::User3dVert(float x, float y, float z)
{
	AddVertex(x, y, z);
	cout << "X: " << x << " Y: " << y << " Z:" << z << endl;

}

void C3DModel::UserMapVertex(float U, float V)
{
	AddTexturePoint(U, V);
	cout << "U: " << U << " V: " << V << endl;
}

void C3DModel::UserCamera(float px, float py, float pz, float tx, float ty, float tz, float Bank, float Lens)
{
	AddCamera(C3DCamera(VECTOR(px, py, pz), VECTOR(tx, ty, tz), Bank, Lens));
}

void C3DModel::UserChunkObj()
{
	if(IsObjectWriting() == true)
		EndObject();
	NewObject();
}

void C3DModel::UserTransformMatrix(const Transform3dsMatrix &Transform, 
								   const Translate3dsMatrix &Translate)
{
	SetTranslation(Translate._11, Translate._12, Translate._13);
	VECTOR	c1(Transform._11, Transform._21, Transform._31);
	VECTOR	c2(Transform._12, Transform._22, Transform._32);
	VECTOR	c3(Transform._13, Transform._23, Transform._33);
	MATRIX	trans(c1, c2, c3);
	SetMatrix(trans);
}

void C3DModel::UserEndOfFile()
{
	if(IsObjectWriting() == true)
		EndObject();
}

void C3DModel::	write3dVert(float x, float y, float z)
{
	 //mObjects[mObjectsCount]->vertices.
		
}