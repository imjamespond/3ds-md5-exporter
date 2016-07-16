#include <string>
#include <vector>
#include <hash_map>
#include <map>
#include <set>
#include <boost\shared_ptr.hpp>
#include <max.h>
namespace MaxData
{
	typedef struct Weight
	{
		Weight():boneId(0),wx(0),wy(0),wz(0),bias(-1),index(-1),boneName(){};

		int index;

		int boneId;
		std::string boneName;
		double bias;
		//position
		double wx;
		double wy;
		double wz;
	} max_weight;
	typedef boost::shared_ptr<max_weight> max_weight_ptr;
	typedef std::vector<max_weight_ptr > max_weight_list;

	typedef struct Vertex
	{
		Vertex():x(0),y(0),z(0),u(0),v(0),nx(0),ny(0),nz(0),index(-1),weightList(){};

		int index;

		// Position
		double x;
		double y;
		double z;

		// UV
		double u;
		double v;

		// Normal
		double nx;
		double ny;
		double nz;

		// weight
		max_weight_list weightList;
		
	} max_vertex;
	class max_vertex_less:public std::less<int>
	{
	public:
		bool operator () (const int i1,const int i2) const
		{
			return i1<i2;
		}
	};
	typedef boost::shared_ptr<max_vertex> max_vertex_ptr;
	typedef std::multimap<int, max_vertex_ptr, max_vertex_less> max_vertex_map;
	typedef std::multimap<int, max_vertex_ptr, max_vertex_less>::iterator max_vertex_map_it;
	typedef std::multimap<int, max_vertex_ptr, max_vertex_less>::const_iterator max_vertex_map_conit;
	typedef struct Index
	{
		Index():v1(0),v2(0),v3(0),index(-1){};

		int index;

		//
		int v1;
		int v2;
		int v3;
	} max_index;
	typedef boost::shared_ptr<max_index> max_index_ptr;
	typedef std::vector<max_index_ptr > max_index_list;

	typedef struct Bone {
		Bone():x(0),y(0),z(0),ox(0),oy(0),oz(0),id(nullptr),pid(nullptr),node(nullptr),pNode(nullptr),index(0),parentIndex(0){};
		std::string name;

		Object* id;
		Object* pid;
		INode* node;
		INode* pNode;
		int index;
		int parentIndex;

		//position
		double x;
		double y;
		double z;

		//orientation
		double ox;
		double oy;
		double oz;

	} max_bone;
	typedef boost::shared_ptr<max_bone> max_bone_ptr;
	typedef std::vector<max_bone_ptr > max_bone_list;
	typedef std::hash_map<std::string, max_bone_ptr > max_bone_map;
	typedef struct Mesh
	{
		Mesh():maxVertexMap(),maxIndexList(){};

		max_vertex_map maxVertexMap;
		max_index_list maxIndexList;	

		std::string name;
		std::string shaderName;
	
		int addMaxVertexList(int, Point3*, Point3*, Point3* );
		void addMaxIndexList(int, int, int, int);
	} max_mesh;
	typedef boost::shared_ptr<max_mesh> max_mesh_ptr;
	typedef std::hash_map<std::string, max_mesh_ptr > max_mesh_map;	
}