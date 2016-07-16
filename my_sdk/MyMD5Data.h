#pragma once
#include <max.h>
#include <IGame/IGame.h>
#include <vector>
#include <algorithm>

using namespace std;

struct BoneInfo 
{
	BoneInfo()
		:
	SelfIndex(-1),
		ParentIndex(-1)
	{

	}
	MCHAR* Name;
	int SelfIndex;
	int ParentIndex;
	Point3 Pos;
	Quat Rot;

	bool operator < (const BoneInfo &b) const
	{
		//父节点小的在前面同时保证名字按顺序
		return MaxAlphaNumComp(Name,b.Name)<0;
	}
};

struct ObjectInfo 
{
	ObjectInfo()
		:StartMesh(0),
		MeshCount(0)
	{

	}

	MCHAR* Name;
	int StartMesh;
	int MeshCount;

};

struct MaterialInfo
{
	MaterialInfo()
		:Diffuse(NULL),
		Opacity(NULL),
		TwoSided(FALSE),
		Transparency(1.0f)
	{

	}
	MCHAR* Name;
	MCHAR* Diffuse;
	MCHAR* Opacity;
	BOOL TwoSided;
	float Transparency;
};

struct WeightInfo;

struct VertexInfo
{
	VertexInfo()
		:VertIndex(-1),
		TexCoordIndex(-1),
		WeightIndex(-1),
		WeightCount(-1)
	{

	}
	int VertIndex;
	int TexCoordIndex;
	Point3 Pos;
	Point2 UV;
	int WeightIndex;
	int WeightCount;

	vector<WeightInfo> Weights;
};

struct TriInfo
{
	int Index[3];
};

struct WeightInfo 
{
	WeightInfo()
		:Value(0.f)
	{

	}
	float Value;
	IGameNode* Bone;
	Point3 Offset;
};

void BoneSort(vector<BoneInfo>& boneList);