// 3DObject.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__INCLUDED_)
#define AFX_3DOBJECT_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "VECTOR.h"
#include "MATRIX.h"
#include "3DFace.h"
#include <vector>
#include <map>

#define HashFn(x) x
#define KW 0.01
#define KQ2 0.01
#define KQ3 0.01
#define KD2 250
#define KD3 250
#define w_i (KD2 - 2)
#define default_k_w 0.0001



class C3DObject
{
public:
	C3DObject();
	~C3DObject();
	void	embeddingAlgo();
	void	write3dsfile();
	float   vertexData(int index,float x, float y, float z);
	bool	Vertex_Body_Table(int index);
	bool	isMarkVertex(float x); 
	float	modulate_x_1_Vertex(float x,bool mark_fstatus);
	void	Eq_3(float *x2C,float *x3C);
	float	Eq_4(float xj,float xjc, float kqj); 
	float   Eq_5(float xj, float xjc, float kjd, float xje,float kjq,float hi_OR_wi);
	float	Eq_6(float xja);
	float	Eq_7(float x23C,float x23_1,float kjq,float kjd);
	
	float	Get_xd2(float x2,float x2c,float wi);
	float	Get_xd3(float x3,float x3c,float wi);
	void	ExtractionAlgo();	

	vector	<VECTOR>	vertices;
	vector	<VECTOR>	texturePoints;
	vector	<C3DFace>	faces;

	
	std::vector<int> neighboringVertices;
	std::map<int, int> adjusting_vmap;
	
	MATRIX	transform;
	float	translate[3];
};

#endif // !defined(AFX_3DOBJECT_H__INCLUDED_)
