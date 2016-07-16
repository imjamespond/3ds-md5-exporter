#pragma once

#include "MyUtils.h"
#include "Md5Data.h"
#include <vector>
#include <boost\shared_ptr.hpp>

extern MyUtils* pMyUtils;

class Md5Anim
{
public:
	typedef boost::shared_ptr<Md5Data::Joint> md5_joint_ptr;
	typedef std::vector<md5_joint_ptr > md5_joint_list;
	md5_joint_list jointList;
	Md5Data::bound_list boundList;
	Md5Data::frame_list frameList;
	Md5Data::frame_unit_list baseFrameList;
	Md5Data::flag_list flagList;

	Md5Anim();
	~Md5Anim(void);

	short numFrames;
	short numJoints;
	short frameRate;
	short numAnimatedComponents;
	short boneCount;
	short animatedCount;

	void doJoint();
	void doExport();

	static char *frameformat;
	static char *baseframeformat;
	static float precision;
	static void setPrecision(int );
};
