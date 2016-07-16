#include "stdafx.h"
#include "Md5Mesh.h"

float gPrecision(1.0f);

void cpyString(char **dst, const std::string &str);
void Md5Mesh::setPrecision(int idx)
{
	if(idx==0)
	{
		gPrecision = 1000.0f;
	}else if(idx==1)
	{
		gPrecision = 10000.0f;
	}else if(idx==2)
	{
		gPrecision = 100000.0f;
	}

	if(idx==3)
	{
		gPrecision = 1000000.0f;
		floatformat="%.6f ";
	}

	std::string s;
	s=s+"\"%s\" %d ( "+floatformat+floatformat+floatformat+") ( "+floatformat+floatformat+floatformat+") // %s";
	cpyString(&jointformat,s);
	s.clear();
	s=s+"vert %d ( "+floatformat+floatformat+") %d %d";
	cpyString(&vertexformat,s);
	s.clear();
	s=s+"weight %d %d "+floatformat+"( "+floatformat+floatformat+floatformat+")";
	cpyString(&weightformat,s);
	s.clear();
}
void cpyString(char **dst, const std::string &str)
{
	size_t len = str.length()+1;//1 for null terminator
	*dst = reinterpret_cast<char *>(::malloc(len));
	::memset(*dst, 0, len);
	_memccpy(*dst, str.c_str(), 0, len);
}

//must initialize vector before use
Md5Mesh::Md5Mesh():
	md5MeshList(),
	jointList(),
	floatformat("%g "),
	jointformat(nullptr),
	vertexformat(nullptr),
	weightformat(nullptr)
{
}

Md5Mesh::~Md5Mesh(void)
{
	if(jointformat)
		delete jointformat;
	if(vertexformat)
		delete vertexformat;
	if(weightformat)
		delete weightformat;
}

void
	Md5Mesh::addMeshData(md5_mesh_data_ptr& p)
{
	md5MeshList.push_back(p);
}

void
	Md5Mesh::doExport(){
		pMyUtils->writeFun(0,"MD5Version 10");
		pMyUtils->writeFun(0,"commandline \"\"\n");

		pMyUtils->writeFun(0,"numJoints %d",jointList.size());
		pMyUtils->writeFun(0,"numMeshes %d\n",md5MeshList.size());

		doJoint();

		md5_mesh_list::iterator it;
		for(it = md5MeshList.begin(); it != md5MeshList.end(); ++it)
		{
			md5_mesh_data_ptr p = *it;

			doMesh(p);
		}
}

void
	Md5Mesh::doJoint(){
		pMyUtils->writeFun(0,"joints {");
		//	"origin"	-1 ( 0 0 0 ) ( -0.7071067095 0 0 )		// 
		md5_joint_list::iterator jIt;
		for(jIt = jointList.begin(); jIt != jointList.end(); ++jIt)
		{
			md5_joint_ptr jointPtr = *jIt;
			pMyUtils->writeFun(1,jointformat,
				jointPtr->name.c_str(),jointPtr->pid, truncf(jointPtr->x), truncf(jointPtr->y), truncf(jointPtr->z), truncf(jointPtr->ox), truncf(jointPtr->oy), truncf(jointPtr->oz), jointPtr->name.c_str());
		}

		pMyUtils->writeFun(0,"}");
}

void
	Md5Mesh::doMesh(md5_mesh_data_ptr& p){
		pMyUtils->writeFun(0,"mesh {");
		pMyUtils->writeFun(1,"// meshes: %s",p->name.c_str());
		pMyUtils->writeFun(1,"shader \"%s\"",p->shaderName.c_str());
		pMyUtils->writeFun(1,"\nnumverts %d",p->vertexList.size());
		Md5Data::vertex_list::iterator vIt;
		for(vIt = p->vertexList.begin(); vIt != p->vertexList.end(); ++vIt)
		{
			doVertex(*vIt);
		}

		pMyUtils->writeFun(1,"\nnumtris  %d",p->triangleList.size());
		Md5Data::triangle_list::iterator tIt;
		for(tIt = p->triangleList.begin(); tIt != p->triangleList.end(); ++tIt)
		{
			doTriangle(*tIt);
		}

		pMyUtils->writeFun(1,"\nnumweights  %d",p->weightList.size());
		Md5Data::weight_list::iterator wIt;
		for(wIt = p->weightList.begin(); wIt != p->weightList.end(); ++wIt)
		{
			doWeight(*wIt);
		}

		pMyUtils->writeFun(0,"}");
}

void
	Md5Mesh::doVertex(Md5Data::vertex_ptr& p){
		pMyUtils->writeFun(1,vertexformat,p->index,truncf(p->u),truncf(p->v),p->wIndex,p->wSize);
}

void
	Md5Mesh::doTriangle(Md5Data::triangle_ptr& p){
		pMyUtils->writeFun(1,"tri %d %d %d %d",p->index, p->vIndex1, p->vIndex2, p->vIndex3);
}

void
	Md5Mesh::doWeight(Md5Data::weight_ptr& p){
		pMyUtils->writeFun(1,weightformat,p->index, p->jIndex, truncf(p->bias), truncf(p->weightX), truncf(p->weightY), truncf(p->weightZ));
}