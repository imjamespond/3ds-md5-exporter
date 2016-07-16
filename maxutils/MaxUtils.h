#pragma once
#include <max.h>
#include <dbgprint.h>
#include <iskin.h>//I_SKIN
#include <modstack.h>//IDerivedObject
#include <CS/BIPEXP.H>
#include <CS/Biped12Api.h>
#include <IGame/IGame.h>
#include <IGame/IConversionManager.h>
#include <IGame/IGameModifier.h>
#include <Md5Mesh.h>
#include <Md5Anim.h>
#include <boost\tuple\tuple.hpp>
#include <MaxData.h>

extern void updateProgress(const char * msg, bool progress=1);
extern float flagPrecision;
extern int gFrameStart;
extern int gFrameEnd;
extern int gWeightNum;
extern float gBJX;
extern float gBJY;
extern float gBJZ;
extern Interface* gInterface;

class MaxUtils
{

public:
	MaxUtils();

	//about mesh
	static int calcNumDescendants(INode* node);
	static void exportNodes(INode* node, MyUtils&);

	void exportSkeletons(INode* );
	void exportTriangles(INode* );
	void convertToMd5Mesh();
	void dump();

	//about animation
	void dumpJoint(INode* node);
	void dumpBounds(INode* node);
	void dumpBaseframe(INode* node);
	void dumpFrame(INode* node);
	void convertToMd5Anim();

	//Game utils
	void gameExportTriangles(IGameMesh * gM );
	void gameDumpMeshes(IGameScene* pIgame);
	void gameDumpMesh(IGameNode* pGameNode);

	GMatrix toRightHand(Matrix3& mat);

	void setMd5Mesh(Md5Mesh* p){pMd5Mesh_=p;}
	Md5Mesh* getMd5Mesh(){return pMd5Mesh_;}
	void setMd5Anim(Md5Anim* p){pMd5Anim_=p;}
private:
	Md5Mesh* pMd5Mesh_;
	Md5Anim* pMd5Anim_;

	MaxData::max_mesh_map meshMap;
	MaxData::max_bone_map maxBoneMap;
	MaxData::max_bone_list maxBoneList;

private:
	//about mesh
	void parseTriangle(INode* node);
	//about animation
	void caculateFlag(Md5Anim::md5_joint_ptr& );

public:
	void addBaseJoint()
	{
		MaxData::max_bone_ptr bonePtr(new MaxData::Bone);		
		bonePtr->name = "base000";
		maxBoneMap[bonePtr->name] = bonePtr;
		maxBoneList.push_back(bonePtr);
	}
private:
	bool isBone(Object *obj,Object *pobj)
	{
		if(obj && 
			(obj->ClassID() == BONE_OBJ_CLASSID 
			|| obj->ClassID() == SKELOBJ_CLASS_ID//bip
			|| (pobj&&pobj->ClassID() == SKELOBJ_CLASS_ID)//nub
			)
			//|| obj->ClassID().PartA() == DUMMY_CLASS_ID)
			//&& HELPER_CLASS_ID != obj->SuperClassID()//exclude nub
			)
			return true;
		else
			return false;
	}
	/*
	取得蒙皮修改器
	*/
	typedef boost::tuple<int, IDerivedObject *> type_skin;
	MaxUtils::type_skin getSkinModIndex(Object *obj)//, IDerivedObject **derivedObject
	{
		if (obj && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID) 
		{
			//遍历结点的所有修改器
			IDerivedObject *derived = static_cast<IDerivedObject *>(obj);
			for (int i=0; i < derived->NumModifiers(); i++) 
			{
				Modifier *mod = derived->GetModifier(i);
				//蒙皮修改器
				void *skin = mod->GetInterface(I_SKIN);
				if (skin)
				{
					type_skin tSkin = boost::make_tuple(i,derived);
					return tSkin;
				}
			}
		}
		type_skin null = boost::make_tuple(-1, static_cast<IDerivedObject *>(NULL));
		return null;
	}
};

