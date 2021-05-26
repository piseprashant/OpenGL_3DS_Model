// 3DObject.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#include <vector>
using namespace std;

#include "VECTOR.h"

#include <algorithm>
#include "MATRIX.h"
#include "3DFace.h"
#include "3DObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DObject::C3DObject()
{

}

C3DObject::~C3DObject()
{
	vertices.clear();
	texturePoints.clear();
	faces.clear();
}


template<typename T>
void  removeDuplicates(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

//void C3DObject::getVertics()
//{
//	return 
//}

void C3DObject::embeddingAlgo()
{
	printf("Encrypting WaterMark in vertices\n"); 
	int vnum = vertices.size();
	int index = 0;
	bool multiAdjstVrtx;
	static float x2C,x3C;
	float x2_a,x3_a;
	//int the_iterator1_index = 0;
//	float x2_e,x3_e;
	//for each vertex vi in the mesh model		
	
	unsigned int activeVrtxIter = 0;

	while(activeVrtxIter < vertices.size())
	{
		multiAdjstVrtx = Vertex_Body_Table(activeVrtxIter);
		// check if v is a mark eligible vertex and there is at least one
		// eligible adjusting vertex among vact’s neighboring vertices
		//(isMarkVertex(vertices[activeVrtxIter].x)
		if(vertices[activeVrtxIter].mark_eligible && multiAdjstVrtx)
		{
			//randomly select one eligible adjusting vertex v j from
			//vact’s neighboring vertices as its adjusting vertex
			int randomIndex = rand() % neighboringVertices.size();

			vertices[activeVrtxIter].mark = true;
			vertices[activeVrtxIter].mark_eligible = false;
			vertices[activeVrtxIter].adjusting_eligible = false;
			vertices[randomIndex].mark_eligible = false;
			
			//adjusting_vmap[activeVrtxIter] = randomIndex;
		    adjusting_vmap.insert(std::pair<int ,int>(activeVrtxIter,randomIndex));  

			//for each vact’s neighboring vertex vi --> vi .adjusting eligible = False			
			std::vector<int>::iterator the_iterator1;
			
		
			for (the_iterator1 = neighboringVertices.begin(); the_iterator1 != neighboringVertices.end(); the_iterator1++)
			{
				vertices[*the_iterator1] .adjusting_eligible = false;
			}
			//calculate the x2 and x3 coordinate barycenters of vact using Eq. (3)
			 Eq_3(&x2C,&x3C);

			// embed watermarks to vact’s x2 and x3 coordinates using Eqs. (4) and (5)
			//x2_e = Eq_4(vertices[activeVrtxIter].y,x2C, KQ2);
			//x3_e = Eq_4(vertices[activeVrtxIter].z,x3C, KQ3);

			
			//Eq_5(vertices[activeVrtxIter].y,x2C,KD2,x2_e,KQ2,hi_OR_wi);
			//Eq_5(vertices[activeVrtxIter].z,x3C,KD3,x3_e,KQ3,hi_OR_wi);
			 vertices[activeVrtxIter].y_1  = Get_xd2(vertices[activeVrtxIter].y,x2C,w_i);
			 vertices[activeVrtxIter].z_1  = Get_xd3(vertices[activeVrtxIter].z,x3C,w_i);
		}
   		else if(multiAdjstVrtx)
		{
			vertices[activeVrtxIter].isolated_vertex = true;
			vertices[activeVrtxIter].mark = true;	
		}
		activeVrtxIter++;
	}

	//for each vertex vi in the mesh model	
	activeVrtxIter = 0;
	int mapVal;
	while(activeVrtxIter < vertices.size())
	{
		multiAdjstVrtx = Vertex_Body_Table(activeVrtxIter);
		//modulate vact’s x1 coordinate according to its mark flag using Eqs. (1) and (2)
		vertices[activeVrtxIter].x_1 = modulate_x_1_Vertex(vertices[activeVrtxIter].x,vertices[activeVrtxIter].mark);
		if(vertices[activeVrtxIter].mark)
		{			
//		    mapVal = adjusting_vmap.find(activeVrtxIter)->second;
//			x2_a = Eq_6(vertices[adjusting_vmap.find(activeVrtxIter)->second].y) ;
//			x3_a = Eq_6(vertices[adjusting_vmap.find(activeVrtxIter)->second].z) ;
		}
		activeVrtxIter++;
	}
	//  For each isolated vertex vi in the mesh model 
	//	calculate the x2 and x3 coordinate barycenters of vi using Eq. (3)
	for(int iIsolatedVrtx = 0;iIsolatedVrtx < vertices.size();iIsolatedVrtx++)
	{ 
		if(vertices[iIsolatedVrtx].isolated_vertex == true)
		{
			Vertex_Body_Table(iIsolatedVrtx);
			Eq_3(&x2C,&x3C);
			//Embed watermarks to vi ’s x2 and x3 coordinates using Eqs. (4) and (5)
			vertices[iIsolatedVrtx].y_1  = Get_xd2(vertices[iIsolatedVrtx].y,x2C,w_i);
			vertices[iIsolatedVrtx].z_1  = Get_xd3(vertices[iIsolatedVrtx].z,x3C,w_i);
		}
	}
}


bool C3DObject :: Vertex_Body_Table(int index)
{
	std::vector<C3DFace>::iterator the_iterator1;
	for (the_iterator1 = faces.begin(); the_iterator1 != faces.end(); the_iterator1++)
	{
		
		if((the_iterator1->A == index) || (the_iterator1->B == index) ||(the_iterator1->C == index))
		{
			neighboringVertices.push_back(the_iterator1->A);
			neighboringVertices.push_back(the_iterator1->B);
			neighboringVertices.push_back(the_iterator1->C);
			
		}
	}
/****************************************************************************************************************/
	// Testing code
	int index1 = 0;
	std::vector<int>::iterator it;
	printf("neighboringVertices are : ");
	for(it = neighboringVertices.begin(); it != neighboringVertices.end(); it++)
	{
		
		printf("%d\t",neighboringVertices[index1]);
		index1++;
	}
	printf("\n");
/****************************************************************************************************************/
	removeDuplicates<int>(neighboringVertices);
/****************************************************************************************************************/	
	// Testing code
	index1 = 0;
	
	printf("final neighboringVertices are : ");
	for(it = neighboringVertices.begin(); it != neighboringVertices.end(); it++)
	{
		
		printf("%d\t",neighboringVertices[index1]);
		index1++;
	}
	printf("\n");
/****************************************************************************************************************/
	if(neighboringVertices.size() > 1)
	{
		vector<int>::iterator f = find(neighboringVertices.begin(), neighboringVertices.end(), index);
		//find and delete the vertex index mentioned
		if( f != neighboringVertices.end() ) {
			neighboringVertices.erase(f);
			printf("\nYou have deleted vertex : %d\n",index);
		}	
		
/****************************************************************************************************************/	
	// Testing code
	index1 = 0;
	
	printf("final neighboringVertices are : ");
	for(it = neighboringVertices.begin(); it != neighboringVertices.end(); it++)
	{
		
		printf("%d\t",neighboringVertices[index1]);
		index1++;
	}
	printf("\n");
/****************************************************************************************************************/
		return true;
	}
	return false;

}

bool  C3DObject :: isMarkVertex(float x) 
{
	int m;
	m = fabs(x/default_k_w);
	if(m % 2)
	{
		//if m = 1 and v is a mark vertex)
		return true;
	}
	return false;
}

float  C3DObject :: modulate_x_1_Vertex(float x,bool mark_fstatus) 
{
	bool isMark =  isMarkVertex(x);
	float result;
	
	if((isMark && mark_fstatus) || (!isMark && !mark_fstatus))
	{
		//if m = 1 and v is a mark vertex)
		return x;
	} 
	else
	{
		result = x + default_k_w;
		return result;		
	}
}

void C3DObject :: Eq_3(float *x2C,float *x3C) 
{
	// For a mark vertex v, its barycenters of x2 and x3 coordinates, xc2 and xc3,
	// are calculated by averaging the coordinates of its neighboring vertices
    int iSizeNv;
	//int the_iterator1_index = neighboringVertices[0];
	static float xj_y,xj_z;
	std::vector<int>::iterator the_iterator1;
	for (the_iterator1 = neighboringVertices.begin(); the_iterator1 != neighboringVertices.end(); the_iterator1++)
	{
		xj_y	+= vertices[*the_iterator1] .y;
		xj_z	+= vertices[*the_iterator1] .z;
		//the_iterator1_index++;
	}
	iSizeNv = neighboringVertices.size();
	*x2C= xj_y/iSizeNv;
	*x3C= xj_z/iSizeNv;
}

float C3DObject ::Get_xd2(float x2,float x2c,float wi)
{
       return Eq_5(x2,x2c,KD2,Eq_4(x2,x2c,KQ2),KQ2,wi);
}

float C3DObject ::Get_xd3(float x3,float x3c,float wi)
{
       return Eq_5(x3,x3c,KD3,Eq_4(x3,x3c,KQ3),KQ3,HashFn(wi));
}

float C3DObject :: Eq_4(float xj,float xjc, float kqj) {
	float xje;
//	xje=fabs(xj-xjc);
	if (xj>xjc)
		xje=(xj-(fmodf(fabs(xj-xjc), kqj)));
	else
		xje=(xj+(fmodf(fabs(xj-xjc), kqj)));
	return xje;
}


float C3DObject :: Eq_5(float xj, float xjc, float kjd, float xje,float kjq,float hi_OR_wi)
{
	float xdj;
	float dj=(kjq/kjd)*hi_OR_wi;
	if(xj>xjc)
	{
		xdj=xje+dj;
	}
	else
	{
		xdj=xje-dj;
	}
	return xdj;
}


float C3DObject :: Eq_6(float xja) 
{
	float sum = 0;
	//int the_iterator1_index = neighboringVertices[0];
	std::vector<int>::iterator the_iterator1;
	for (the_iterator1 = neighboringVertices.begin(); the_iterator1 != neighboringVertices.end(); the_iterator1++)
	{
		sum += vertices[*the_iterator1].x_1 - vertices[*the_iterator1].x;
    }
	return (xja - sum);
}

void C3DObject :: ExtractionAlgo()
{
	//set all vertices in the mesh model as non-suspicious vertices
	//set v1 as the active vertex, vact
	unsigned int activeVrtxIter = 0;
	float x2C,x3C;
	float w_1,h_1;
	//int the_iterator1_index;
	while(activeVrtxIter < vertices.size())
	{
		if(vertices[activeVrtxIter].mark == true)
		{
			// calculate the x2 and x3 coordinate barycenters of
			// vact using Eq. (3)
			//calculate the x2 and x3 coordinate barycenters of vact using Eq. (3)
			 Eq_3(&x2C,&x3C);
			 // extract w0 and h0 from vact’s x2 and x3 coordinates using Eq. (7)
			 w_1 = Eq_7(x2C,vertices[activeVrtxIter].y_1,KQ2,KD2);
			 h_1 = Eq_7(x3C,vertices[activeVrtxIter].z_1,KQ3,KD3);
			 //if h(w0)! = h0 then
			 //set vact and all its neighboring vertices as suspicious vertices
			 if(HashFn(w_1) != h_1)
			 {
				 //set vact and all its neighboring vertices as suspicious vertices
				 vertices[activeVrtxIter].suspicious_vertex = true;
				 Vertex_Body_Table(activeVrtxIter);
				 //the_iterator1_index = neighboringVertices[0];
				 std::vector<int>::iterator the_iterator1;
				 for (the_iterator1 = neighboringVertices.begin(); the_iterator1 != neighboringVertices.end(); the_iterator1++)
				 {	 
					 vertices[*the_iterator1].suspicious_vertex = true;
					 //the_iterator1_index++;
				 }
			 }
		}
//		set the next vertex as the active vertex, vact (Traverse the whole model)
		activeVrtxIter++;
	}
}


float C3DObject :: Eq_7(float x23C,float x23_1,float kjq,float kjd) 
{
	float result;
	result=(fmodf(fabs(x23C-x23_1), kjq) * (kjd/kjq));
	return result; 
}

void C3DObject :: write3dsfile()
{
	//create a temp file of the original 3ds file

//	File *fd;

}

float  C3DObject :: vertexData(int index,float x, float y, float z)
{
	x = vertices[index].x_1;
	y = vertices[index].y_1;
	z = vertices[index].z_1;
	return 0;
}