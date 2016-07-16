#pragma once
#include <vector>
#include <string>
#include <boost\shared_ptr.hpp>

namespace Md5Data
{
	
	enum HierarchyFlag
	{
		enum_None=0,
		enum_Pos_X=0x00000001,
		enum_Pos_Y=0x00000002,
		enum_Pos_Z=0x00000004,
		enum_Rot_X=0x00000008,
		enum_Rot_Y=0x00000010,
		enum_Rot_Z=0x00000020,
	};


	struct Vertex;
	struct Triangle;
	struct Weight;

	typedef boost::shared_ptr<Vertex> vertex_ptr;
	typedef boost::shared_ptr<Triangle> triangle_ptr;
	typedef boost::shared_ptr<Weight> weight_ptr;
	typedef std::vector<vertex_ptr > vertex_list;
	typedef std::vector<triangle_ptr > triangle_list;
	typedef std::vector<weight_ptr > weight_list;
	

	struct Mesh {
		std::string name;
		std::string shaderName;

		vertex_list vertexList;
		triangle_list triangleList;
		weight_list weightList;
	};

	struct Joint {
		Joint():x(0.f),y(0.f),z(0.f),ox(0.f),oy(0.f),oz(0.f),id(-1),pid(-1),index(0),flag(0){};
		std::string name;

		int id;
		int pid;

		//position
		double x;
		double y;
		double z;

		//orientation
		double ox;
		double oy;
		double oz;

		//index
		int index;
		//animation
		int flag;
	};

	struct Vertex {
		Vertex():index(-1),wIndex(0),wSize(0){};

		int index;

		// UV
		double u;
		double v;

		//weight index/size
		int wIndex;
		int wSize;
	};


	struct Triangle{
		Triangle():index(-1){};

		int index;

		//Vertex index
		int vIndex1;
		int vIndex2;
		int vIndex3;
	};

	struct Weight{
		Weight():index(-1),jIndex(0),bias(0),weightX(0),weightY(0),weightZ(0){};

		int index;

		//Joint index
		int jIndex;

		//bias
		double bias;

		//Vertex weight
		double weightX;
		double weightY;
		double weightZ;
	};

	//¶¯»­²¿·Ö

	struct Bound;
	struct Frame;
	struct FrameUnit;
	struct Flag;

	typedef boost::shared_ptr<Bound> bound_ptr;
	typedef boost::shared_ptr<FrameUnit> frame_unit_ptr;
	typedef boost::shared_ptr<Frame> frame_ptr;
	typedef boost::shared_ptr<Flag> flag_ptr;
	typedef std::vector<bound_ptr > bound_list;
	typedef std::vector<frame_unit_ptr > frame_unit_list;
	typedef std::vector<frame_ptr > frame_list;
	typedef std::vector<flag_ptr > flag_list;

	struct Bound {
		float minx;
		float miny;
		float minz;
		float maxx;
		float maxy;
		float maxz;
	};

	struct FrameUnit {
		FrameUnit():x(0),y(0),z(0),qx(0),qy(0),qz(0),id(-1){};
		float x;
		float y;
		float z;
		float qx;
		float qy;
		float qz;
		int id;
	};

	struct Frame {
		Frame():frameUnitList(){};
		int id;
		frame_unit_list frameUnitList;
	};

	struct Flag {
		Flag():boneName(),parentMd5Name() {};
		std::string boneName ;
		int pIndex ;
		int boneFlag ;
		int boneFrameIndex ;
		std::string parentMd5Name ;
	};
};

