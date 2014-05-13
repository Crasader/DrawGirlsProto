//
//  bustMorphing.cpp
//  DGproto
//
//  Created by LitQoo on 2014. 3. 18..
//
//

#include "bustMorphing.h"

void MyNode::triangulationWithPoints(const vector<Vertex3D>& points)
{
	DelaunayTriangulation delaunay;
	for(auto& i : points)
	{
		delaunay.PushVertex(i.x, i.y, i.z);
	}

	vector<Triangle> mTriangles2 = delaunay.getTriangles();

	int nTri = mTriangles2.size();
	m_triCount = nTri;
	if(m_textCoords)
	{
		delete [] m_textCoords;
	}
	if(m_vertices)
		delete [] m_vertices;
	if(m_2xVertices)
		delete [] m_2xVertices;
	if(m_colors)
		delete [] m_colors;
	
	m_textCoords = new Vertex3D[nTri * 3];
	m_vertices = new Vertex3D[nTri * 3];
	m_2xVertices = new Vertex3D[nTri * 3];
	m_colors = new ccColor4F[nTri * 3];
	for(int i=0; i<nTri; i++){
		for(int j=0; j<3; j++){
			auto tempVertices = delaunay.getVertices();
			Vertex3D temp = Vertex3DMake(tempVertices[mTriangles2[i].vt[j]].x / 2.f,
																	 tempVertices[mTriangles2[i].vt[j]].y / 2.f,
																	 tempVertices[mTriangles2[i].vt[j]].extraData);
			//				temp.z = points
			m_vertices[i*3 + j] = temp;
			m_2xVertices[i * 3 + j] = Vertex3DMake(tempVertices[mTriangles2[i].vt[j]].x,
																						 tempVertices[mTriangles2[i].vt[j]].y,
																						 tempVertices[mTriangles2[i].vt[j]].extraData);
			m_textCoords[i*3 + j] = Vertex3DMake(m_2xVertices[i * 3 + j].x / (2 * m_halfWidth),
																					 1.f - (m_2xVertices[i * 3 + j].y) / (2 * m_halfHeight), temp.z);
#if 0
			// 이 코드가 도대체 왜 있는지 모르겠음. 지금은 없어져도 될 듯 함~
			m_colors[i*3 + j] = ccc4f(ks19937::getFloatValue(0, 1), ks19937::getFloatValue(0, 1), ks19937::getFloatValue(0, 1), 1.f);
#endif
		}
	}

	
	
	
	for(int i=0; i<nTri; i++){
		for(int j=0; j<3; j++){
			m_backupVertices[&m_vertices[i*3 + j]] = m_vertices[i*3 + j];
		}
	}

}