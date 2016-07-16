#pragma once

#include "MyUtils.h"
#include "Md5Data.h"
#include <vector>
#include <boost\shared_ptr.hpp>

extern MyUtils* pMyUtils;

class Md5Mesh
{
public:
	Md5Mesh();
	~Md5Mesh(void);

	typedef boost::shared_ptr<Md5Data::Mesh> md5_mesh_data_ptr;
	typedef boost::shared_ptr<Md5Data::Joint> md5_joint_ptr;
	void addMeshData(md5_mesh_data_ptr&);

	void doExport();
	void doJoint();
	void doMesh(md5_mesh_data_ptr &);

	void doVertex(Md5Data::vertex_ptr &);
	void doTriangle(Md5Data::triangle_ptr &);
	void doWeight(Md5Data::weight_ptr &);

	typedef std::vector<md5_mesh_data_ptr> md5_mesh_list;
	md5_mesh_list md5MeshList;
	typedef std::vector<md5_joint_ptr > md5_joint_list;
	md5_joint_list jointList;

	const char *floatformat;
	char *jointformat;
	char *vertexformat;
	char *weightformat;
	void setPrecision(int );
};

