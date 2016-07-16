#include <cmath>
#include <MaxUtils.h>
#include <boost/format.hpp>

int getFlagOffset(int );
void 
	MaxUtils::convertToMd5Anim()
{
	pMd5Anim_->jointList.clear();
	//¼ÆËãbone index
	MaxData::max_bone_list::iterator boneIt;

	int startIndex=0;
	for(boneIt = maxBoneList.begin(); boneIt != maxBoneList.end(); ++boneIt)
	{
		std::string s(boost::str(boost::format("convertToMd5Anim %d ")%(boneIt-maxBoneList.begin())));		
		updateProgress(s.c_str(),0);

		MaxData::max_bone_ptr bonePtr = *boneIt;
		Md5Anim::md5_joint_ptr jointPtr(new Md5Data::Joint);
		jointPtr->name = bonePtr->name;
		jointPtr->x = bonePtr->x;
		jointPtr->y = bonePtr->y;
		jointPtr->z = bonePtr->z;
		jointPtr->ox = bonePtr->ox;
		jointPtr->oy = bonePtr->oy;
		jointPtr->oz = bonePtr->oz;
		jointPtr->pid = bonePtr->parentIndex;
		jointPtr->id = bonePtr->index;
		pMd5Anim_->jointList.push_back(jointPtr);

		//if(bonePtr->parentIndex>=0)
		{
			caculateFlag(jointPtr);//jointPtr->flag = 63;//0 if it is a origin bone,no corresponeding data in frame 
			jointPtr->index = jointPtr->flag>0?startIndex:0;
			startIndex+=getFlagOffset(jointPtr->flag);
		}
	}	
	pMd5Anim_->numAnimatedComponents=startIndex;
	maxBoneMap.clear();
}

int getFlag(const Md5Data::frame_unit_ptr& frameUnitPtr, const Md5Data::frame_unit_ptr& baseFrameUnitPtr);
void 
	MaxUtils::caculateFlag(Md5Anim::md5_joint_ptr& jointPtr)
{
	//baseframe 
	Md5Data::frame_unit_list::iterator bfIt;
	for(bfIt = pMd5Anim_->baseFrameList.begin(); bfIt != pMd5Anim_->baseFrameList.end(); ++bfIt)
	{
		Md5Data::frame_unit_ptr baseFrameUnitPtr = *bfIt;
		if(jointPtr->id==baseFrameUnitPtr->id)
		{//DebugPrint(_T("\nbase unit frame id:%d"),baseFrameUnitPtr->id);
			Md5Data::frame_list::iterator fLiIt;
			for(fLiIt = pMd5Anim_->frameList.begin(); fLiIt != pMd5Anim_->frameList.end(); ++fLiIt)
			{//DebugPrint(_T("\nframe id:%d"),(*fLiIt)->id);
				Md5Data::frame_ptr framePtr = *fLiIt;
				Md5Data::frame_unit_list::iterator fIt;
				for(fIt = framePtr->frameUnitList.begin(); fIt != framePtr->frameUnitList.end(); ++fIt)
				{
					Md5Data::frame_unit_ptr frameUnitPtr = *fIt;
					if(frameUnitPtr->id==baseFrameUnitPtr->id)
					{//DebugPrint(_T("\nframe unit id:%d"),frameUnitPtr->id);
						jointPtr->flag|=getFlag(frameUnitPtr, baseFrameUnitPtr);
					}
				}
			}
		}
	}
}
bool isFlag(float val, float baseVal);
int getFlag(const Md5Data::frame_unit_ptr& frameUnitPtr, const Md5Data::frame_unit_ptr& baseFrameUnitPtr)
{
	int flag(0);
	if(isFlag(frameUnitPtr->x, baseFrameUnitPtr->x))
	{
		flag|=1<<0;
	}
	if(isFlag(frameUnitPtr->y, baseFrameUnitPtr->y))
	{
		flag|=1<<1;
	}
	if(isFlag(frameUnitPtr->z, baseFrameUnitPtr->z))
	{
		flag|=1<<2;
	}
	if(isFlag(frameUnitPtr->qx, baseFrameUnitPtr->qx))
	{
		flag|=1<<3;
	}
	if(isFlag(frameUnitPtr->qy, baseFrameUnitPtr->qy))
	{
		flag|=1<<4;
	}
	if(isFlag(frameUnitPtr->qz, baseFrameUnitPtr->qz))
	{
		flag|=1<<5;
	}
	return flag;
}

bool isFlag(float val, float baseVal)
{
	//DebugPrint(_T("\nbase:%.10f, val:%.10f"), baseVal, val);
	if(std::fabs(val-baseVal)>flagPrecision){
		//DebugPrint(_T("___"));
		return true;
	}
	return false;	
}
int getFlagOffset(int flag)
{
	int offset(0);
	if(flag&1)
	{
		offset++;
	}
	if(flag&2)
	{
		offset++;
	}

	if(flag&4)
	{
		offset++;
	}
	if(flag&8)
	{
		offset++;
	}
	if(flag&16)
	{
		offset++;
	}
	if(flag&32)
	{
		offset++;
	}
	return offset;
}