#include "MaxUtils.h"
#include <boost/format.hpp>
#include <CS/BIPEXP.H>

MaxUtils::MaxUtils(){}

void
	MaxUtils::dumpJoint(INode* node)
{
	Object *obj = node->GetObjectRef();
	INode* pnode = node->GetParentNode();
	Object *pobj = nullptr;
	if(pnode)
	{
		pobj = pnode->GetObjectRef();
	}
	//check obj is not NULL and is a bone
	if (isBone(obj, pobj))
	{
		//collect all bones
		MaxData::max_bone_ptr bonePtr(new MaxData::Bone);		
		bonePtr->name = MyUtils::widechar2str(node->GetName());
		bonePtr->node = node;
		bonePtr->id = obj;
		bonePtr->index = static_cast<int>(maxBoneList.size());//index in maxBoneList
		maxBoneMap[bonePtr->name] = bonePtr;
		maxBoneList.push_back(bonePtr);

		bonePtr->pNode = pnode;
		//铰点Map中找出父铰点
		std::string parentBoneName = MyUtils::widechar2str(pnode->GetName());
		MaxData::max_bone_ptr parentBonePtr = maxBoneMap[parentBoneName];
		bonePtr->parentIndex = parentBonePtr?parentBonePtr->index:0;

	}

	int i;	
	for (i=0; i<node->NumberOfChildren(); i++) 
	{
		dumpJoint(node->GetChildNode(i));
	}
}
/*
void
	MaxUtils::dumpBounds(INode* node)
{
	Object *obj = node->EvalWorldState(0).obj;

	if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{

		//找到对应的网格模型find corresponding mesh
		std::string name = MyUtils::widechar2str(node->GetName());
		MaxData::max_mesh_map::iterator it = meshMap.find(name);

		//找出mesh对应的修改器
		ObjectState os;
		type_skin tSkin = getSkinModIndex(node->GetObjectRef());
		IDerivedObject *derivedObject = tSkin.get<1>();		
		int skinIdx = tSkin.get<0>();
		if (skinIdx >= 0) 
		{
			// Flatten all modifiers up to but not including
			// the skin modifier.
			os = derivedObject->Eval(0, skinIdx + 1);
		}
		else 
		{
			// Flatten entire modifier stack
			os = node->EvalWorldState(0);
		}

		obj = os.obj;
		if (obj && obj->CanConvertToType(triObjectClassID)) 
		{
			//对象
			// Check if there is a skin, that can be
			// exported as part of the geometry.
			ISkin *skin = NULL;
			if (derivedObject && skinIdx >= 0) 
			{
				//取得修改器
				Modifier *mod = derivedObject->GetModifier(skinIdx);
				skin = (ISkin *)mod->GetInterface(I_SKIN);

				if(skin && skin->GetNumBones())
				{

					DebugPrint(_T("dumpBounds:%s\n"), node->GetName());

					int ticksPerFrame = GetTicksPerFrame();

					MaxData::max_bone_ptr originBone = maxBoneList.at(0);
					//set time timerange[1]

					Box3 objBound;
					for(int f=START_FRAME; f<END_FRAME; f++)
					{
						TimeValue t = f * ticksPerFrame;

						const ObjectState& objState=node->EvalWorldState(t);
						Object* maxObj=objState.obj;
						Box3 bb;
						Matrix3 mat=node->GetNodeTM(t);
						maxObj->GetDeformBBox(t,bb,&mat);
						objBound+=bb;

						Point3 min=objBound.Min();
						Point3 max=objBound.Max();
						Md5Data::bound_ptr boundPtr(new Md5Data::Bound);
						boundPtr->minx = (min.x - originBone->x);
						boundPtr->miny = (min.y - originBone->y);
						boundPtr->minz = (min.z - originBone->z);
						boundPtr->maxx = (max.x - originBone->x);
						boundPtr->maxy = (max.y - originBone->y);
						boundPtr->maxz = (max.z - originBone->z);
						pMd5Anim_->boundList.push_back(boundPtr);
					}

				}
			}

		}
	}

	int i;	
	for (i=0; i<node->NumberOfChildren(); i++) 
	{
		dumpBounds(node->GetChildNode(i));
	}
}
*/

void MaxUtils::dumpBaseframe(INode* )
{

	//baseframe 
	MaxData::max_bone_list::iterator bIt;
	for(bIt = maxBoneList.begin(); bIt != maxBoneList.end(); ++bIt)
	{
		Md5Data::frame_unit_ptr frameUnit(new Md5Data::FrameUnit);
		MaxData::max_bone_ptr bonePtr = *bIt;

		if(bonePtr->node)//!=nullptr
		{
			//caculate base pos
			//Matrix3 tm = node->GetNodeTM(0) * Inverse(node->GetParentTM(0));
			Matrix3 tm;
			tm.IdentityMatrix();
			if(bonePtr->pNode)
			{
				tm = Inverse(bonePtr->pNode->GetNodeTM(0));
			}

			tm = bonePtr->node->GetNodeTM(0) * tm;
			GMatrix gtm = GMatrix(tm);// = this->toRightHand(mtx);
			Point3 pos = gtm.Translation();		
			frameUnit->x = pos.x ;
			frameUnit->y = pos.y ;
			frameUnit->z = pos.z ;
			Quat quat = gtm.Rotation();
			//what for?
			if (quat.w<0)
			{
				quat=-quat;
			}
			frameUnit->qx = quat.x;
			frameUnit->qy = quat.y;
			frameUnit->qz = quat.z;	
		}
		frameUnit->id = bonePtr->index;
		pMd5Anim_->baseFrameList.push_back(frameUnit);
	}
}

void MaxUtils::dumpFrame(INode* )
{
	float x(0.0),y(0.0),z(0.0);

	int ticksPerFrame(GetTicksPerFrame());
	int id(0);
	//由第一帧开始
	for(int f=gFrameStart; f<gFrameEnd; f++)
	{
		std::string s(boost::str(boost::format("dumpFrame %d")%f));		
		updateProgress(s.c_str(),0);

		//新建帧
		TimeValue t = f * ticksPerFrame;
		Md5Data::frame_ptr frame(new Md5Data::Frame);
		frame->id = id++;

		//基骨骼偏移
		if(gBJX||gBJY||gBJZ)
		{
			Md5Data::frame_unit_ptr frameUnit(new Md5Data::FrameUnit);
			frameUnit->x = x;
			frameUnit->y = y;
			frameUnit->z = z;
			x+=gBJX;y+=gBJY;z+=gBJZ;

			frameUnit->id = 0;//总为零
			frame->frameUnitList.push_back(frameUnit);
		}

		//遍历所有骨骼
		MaxData::max_bone_list::iterator bIt;
		for(bIt = maxBoneList.begin(); bIt != maxBoneList.end(); ++bIt)
		{
			MaxData::max_bone_ptr bonePtr = *bIt;

			if(bonePtr->node)//!=nullptr
			{
				//caculate
				Matrix3 tm;
				tm.IdentityMatrix();
				if(bonePtr->pNode)
				{
					tm = Inverse(bonePtr->pNode->GetNodeTM(t));
				}
				tm = bonePtr->node->GetNodeTM(t) * tm;
				GMatrix gtm = GMatrix(tm);// = this->toRightHand(mtx);
				Point3 p = gtm.Translation();
				Quat q = gtm.Rotation();
				//what for?
				if (q.w<0)
				{
					q=-q;
				}

				Md5Data::frame_unit_ptr frameUnit(new Md5Data::FrameUnit);
				frameUnit->x = p.x;
				frameUnit->y = p.y;
				frameUnit->z = p.z;
				frameUnit->qx = q.x;
				frameUnit->qy = q.y;
				frameUnit->qz = q.z;

				frameUnit->id = bonePtr->index;
				frame->frameUnitList.push_back(frameUnit);
			}
		}//each bone
		pMd5Anim_->frameList.push_back(frame);
	}

}

void
	MaxUtils::dump()
{
	Interval animation = gInterface->GetAnimRange();
	pMd5Anim_->numFrames=gFrameEnd-gFrameStart;
	pMd5Anim_->boneCount=0;
	pMd5Anim_->frameRate=GetFrameRate();

	addBaseJoint();

	INode *root = gInterface->GetRootNode();

	updateProgress("dumpJoint");
	dumpJoint(root);
	//dumpBounds(root);
	updateProgress("dumpBaseframe");
	dumpBaseframe(root);
	updateProgress("dumpFrame");
	dumpFrame(root);
	updateProgress("convertToMd5Anim");
	convertToMd5Anim();

	updateProgress("doExport");
	pMd5Anim_->doExport();
}
