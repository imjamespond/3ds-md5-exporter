#include "stdafx.h"
//#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "Md5Anim.h"

char *Md5Anim::frameformat("");
char *Md5Anim::baseframeformat("");
float Md5Anim::precision(1.0f);
void Md5Anim::setPrecision(int idx)
{
	frameformat="%g ";
	baseframeformat="( %g %g %g ) ( %g %g %g )";
	if(idx==0)
	{
		gPrecision = 100.0f;
	}else if(idx==1)
	{
		gPrecision = 1000.0f;
	}else if(idx==2)
	{
		gPrecision = 10000.0f;
	}else if(idx==3)
	{
		gPrecision = 100000.0f;
	}else if(idx==4)
	{
		gPrecision = 1000000.0f;
	}
	
	if(idx==5)
	{
		Md5Anim::precision = 10000.0f*10000.0f;
		frameformat="%.12f ";
		baseframeformat="( %.8f %.8f %.8f ) ( %.8f %.8f %.8f )";
	}
}

//must initialize vector before use
Md5Anim::Md5Anim(){}

Md5Anim::~Md5Anim(void){}

void doFrameJoint(MyUtils*, const Md5Data::frame_ptr &, const Md5Anim::md5_joint_list &);
void
	Md5Anim::doExport(){
		numJoints = static_cast<int>(jointList.size());

		pMyUtils->writeFun(0,"MD5Version 10");
		pMyUtils->writeFun(0,"commandline \"\"\n");

		pMyUtils->writeFun(0,"numFrames %d", numFrames);
		pMyUtils->writeFun(0,"numJoints %d", numJoints);
		pMyUtils->writeFun(0,"frameRate %d", frameRate);
		pMyUtils->writeFun(0,"numAnimatedComponents %d", numAnimatedComponents);

		doJoint();

		//bounds
		//pMyUtils->writeFun(0, "bounds {" );
		//Md5Data::bound_list::iterator bIt;
		//for(bIt = boundList.begin(); bIt != boundList.end(); ++bIt)
		//{
		//	Md5Data::bound_ptr boundPtr = *bIt;
		//	pMyUtils->writeFun(1, "( %.10f %.10f %.10f ) ( %.10f %.10f %.10f )",
		//		boundPtr->minx, boundPtr->miny, boundPtr->minz,
		//		boundPtr->maxx, boundPtr->maxy, boundPtr->maxz);
		//}
		//pMyUtils->writeFun(0, "}\n");

		//baseframe 
		pMyUtils->writeFun(0,"baseframe {");
		Md5Data::frame_unit_list::iterator bfIt;
		for(bfIt = baseFrameList.begin(); bfIt != baseFrameList.end(); ++bfIt)
		{
			Md5Data::frame_unit_ptr framePtr = *bfIt;
			pMyUtils->writeFun(1,Md5Anim::baseframeformat, truncf(framePtr->x),truncf(framePtr->y),truncf(framePtr->z),truncf(framePtr->qx),truncf(framePtr->qy),truncf(framePtr->qz));
		}
		pMyUtils->writeFun(0,"}" );

		//frames
		Md5Data::frame_list::iterator fLiIt;
		for(fLiIt = frameList.begin(); fLiIt != frameList.end(); ++fLiIt)
		{
			Md5Data::frame_ptr framePtr = *fLiIt;
			pMyUtils->writeFun(0,"frame %d {", (framePtr->id));
			doFrameJoint(pMyUtils,framePtr,jointList);
			pMyUtils->writeFun(0,"}" );
		}
}

void
	Md5Anim::doJoint(){

		pMyUtils->writeFun(0, "hierarchy {");
		md5_joint_list::iterator jIt;

		for(jIt = jointList.begin(); jIt != jointList.end(); ++jIt)
		{
			md5_joint_ptr jointPtr = *jIt;
			md5_joint_ptr pJointPtr;
			std::string parentmd5name;
			if(jointPtr->pid<0)
			{
				parentmd5name = "";
			}else
			{
				pJointPtr = jointList.at(jointPtr->pid);
				if(pJointPtr)
				{
					parentmd5name = pJointPtr->name;
				}else
				{
					parentmd5name = "null";
				}
			}


			pMyUtils->writeFun(1,"\"%s\" %d %d %d // %s",
				jointPtr->name.c_str(),jointPtr->pid, jointPtr->flag, jointPtr->index, parentmd5name.c_str());
		}
		pMyUtils->writeFun(0,"}");
}

void doFrameJoint(MyUtils* pUtils, const Md5Data::frame_ptr &framePtr,  const Md5Anim::md5_joint_list &jointList)
{
	Md5Data::frame_unit_list::iterator fIt;
	for(fIt = framePtr->frameUnitList.begin(); fIt != framePtr->frameUnitList.end(); ++fIt)
	{
		Md5Data::frame_unit_ptr frameUnitPtr = *fIt;
		//找出当前帧每个节点的flag
		Md5Anim::md5_joint_list::const_iterator jIt;	
		for(jIt = jointList.begin(); jIt != jointList.end(); ++jIt)
		{
			Md5Anim::md5_joint_ptr jointPtr = *jIt;
			if(jointPtr->id==frameUnitPtr->id)
			{
				std::string s;
				if(jointPtr->flag&1)
				{
					s += boost::str(boost::format(Md5Anim::frameformat)%truncf(frameUnitPtr->x));			
				}
				if(jointPtr->flag&2)
				{
					s += boost::str(boost::format(Md5Anim::frameformat)%truncf(frameUnitPtr->y));
				}
				if(jointPtr->flag&4)
				{
					s += boost::str(boost::format(Md5Anim::frameformat)%truncf(frameUnitPtr->z));
				}
				if(jointPtr->flag&8)
				{
					s += boost::str(boost::format(Md5Anim::frameformat)%truncf(frameUnitPtr->qx));
				}
				if(jointPtr->flag&16)
				{
					s += boost::str(boost::format(Md5Anim::frameformat)%truncf(frameUnitPtr->qy));
				}
				if(jointPtr->flag&32)
				{
					s += boost::str(boost::format(Md5Anim::frameformat)%truncf(frameUnitPtr->qz));
				}
				if(jointPtr->flag>0)
					pUtils->writeFun(1,"%s", s.c_str());
			}
		}
	}


}