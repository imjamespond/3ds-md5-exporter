#include "MaxUtils.h"
#include <boost/format.hpp>

class weight_comp;
void addWeight(Md5Data::weight_list& weightList, 
			   MaxData::max_weight_list& maxWeightList, 
			   int& weightIndex, 
			   MaxData::max_bone_map& maxBoneMap);

void 
	MaxUtils::convertToMd5Mesh()
{
	pMd5Mesh_->jointList.clear();
	//计算bone index
	MaxData::max_bone_list::iterator boneIt;

	for(boneIt = maxBoneList.begin(); boneIt != maxBoneList.end(); ++boneIt)
	{
		MaxData::max_bone_ptr bonePtr = *boneIt;
		Md5Mesh::md5_joint_ptr jointPtr(new Md5Data::Joint);
		jointPtr->name = bonePtr->name;
		jointPtr->x = bonePtr->x;
		jointPtr->y = bonePtr->y;
		jointPtr->z = bonePtr->z;
		jointPtr->ox = bonePtr->ox;
		jointPtr->oy = bonePtr->oy;
		jointPtr->oz = bonePtr->oz;
		jointPtr->pid = bonePtr->parentIndex;
		pMd5Mesh_->jointList.push_back(jointPtr);
	}

	//Each Mesh
	int count(0);
	MaxData::max_mesh_map::iterator it;
	for(it=meshMap.begin(); it!=meshMap.end(); ++it)
	{
		std::string s(boost::str(boost::format("convertToMd5Mesh %d ")%(count)));		
		updateProgress(s.c_str(),0);

		//Md5Mesh
		Md5Mesh::md5_mesh_data_ptr md5MeshDataPtr(new Md5Data::Mesh());
		this->pMd5Mesh_->addMeshData(md5MeshDataPtr);

		MaxData::max_mesh_ptr& p = it->second;
		//clean
		md5MeshDataPtr->vertexList.clear();
		md5MeshDataPtr->triangleList.clear();
		md5MeshDataPtr->weightList.clear();

		//每个顶点,及其的权重List
		int weightIndex = 0;
		typedef std::hash_map<int, int> old_new_index_map;
		MaxData::max_vertex_map::iterator vertexIt;
		for(vertexIt = p->maxVertexMap.begin(); vertexIt != p->maxVertexMap.end(); ++vertexIt)
		{
			MaxData::max_vertex_ptr maxVertexPtr = vertexIt->second;
			Md5Data::vertex_ptr md5VertexPtr(new Md5Data::Vertex);
			md5VertexPtr->index = maxVertexPtr->index;//非原始的索引
			md5VertexPtr->u = maxVertexPtr->u;
			md5VertexPtr->v = maxVertexPtr->v;
			md5VertexPtr->wIndex = weightIndex;
			int boneNum = static_cast<int>(maxVertexPtr->weightList.size());
			md5VertexPtr->wSize = boneNum > gWeightNum ? gWeightNum : boneNum;
			md5MeshDataPtr->vertexList.push_back(md5VertexPtr);

			/*
			导出权重
			*/
			addWeight(md5MeshDataPtr->weightList, maxVertexPtr->weightList, weightIndex, maxBoneMap);
		}//traverse all vertexes 

		/*
		导出索引
		*/
		MaxData::max_index_list::iterator indexIt;
		for(indexIt = p->maxIndexList.begin(); indexIt != p->maxIndexList.end(); ++indexIt)
		{
			MaxData::max_index_ptr indexPtr = *indexIt;
			Md5Data::triangle_ptr trianglePtr(new Md5Data::Triangle);

			trianglePtr->index = indexPtr->index;
			trianglePtr->vIndex1 = indexPtr->v1;
			trianglePtr->vIndex2 = indexPtr->v2;
			trianglePtr->vIndex3 = indexPtr->v3;
			md5MeshDataPtr->triangleList.push_back(trianglePtr);
		}


		//clean
		//p->maxVertexMap.clear();
		//p->maxIndexList.clear();

	}

	
	maxBoneMap.clear();
}

//由大到小排
class weight_comp:public std::less<double>
{
public:
	bool operator () (const double i1,const double i2) const
	{
		return i1>i2;
	}
};
typedef std::multimap<double, MaxData::max_weight_ptr, weight_comp> weight_comp_map;
typedef std::pair<double, MaxData::max_weight_ptr> weight_comp_pair;
void 
	addWeight(Md5Data::weight_list& weightList, MaxData::max_weight_list& maxWeightList, int& weightIndex, MaxData::max_bone_map& maxBoneMap)
{
	weight_comp_map weightCompMap;
	MaxData::max_weight_list::iterator weightIt;
	for(weightIt=maxWeightList.begin(); weightIt!=maxWeightList.end(); ++weightIt)
	{
		MaxData::max_weight_ptr maxWeightPtr = *weightIt;		
		weightCompMap.insert(weight_comp_pair(maxWeightPtr->bias,maxWeightPtr));		
	}	
	//更新权重
	float totalWeight(0.0);//有效的总权重值
	int boneNum(0);	
	weight_comp_map::iterator wIt;
	for(wIt=weightCompMap.begin(); wIt!=weightCompMap.end()&&boneNum<gWeightNum; ++wIt,++boneNum)
	{
		MaxData::max_weight_ptr maxWeightPtr = wIt->second; 
		totalWeight+=maxWeightPtr->bias;
	}

	boneNum = 0;	
	for(wIt=weightCompMap.begin(); wIt!=weightCompMap.end()&&boneNum<gWeightNum; ++wIt,++boneNum)
	{
		MaxData::max_weight_ptr maxWeightPtr = wIt->second; 
		Md5Data::weight_ptr md5WeightPtr(new Md5Data::Weight);
		md5WeightPtr->index = weightIndex++;
		//md5WeightPtr->jIndex = maxWeightPtr->boneId;
		md5WeightPtr->bias = maxWeightPtr->bias/totalWeight;
		md5WeightPtr->weightX = maxWeightPtr->wx;
		md5WeightPtr->weightY = maxWeightPtr->wy;
		md5WeightPtr->weightZ = maxWeightPtr->wz;

		MaxData::max_bone_map::const_iterator bit = maxBoneMap.find(maxWeightPtr->boneName);
		if(bit!=maxBoneMap.end())
		{
			MaxData::max_bone_ptr bonePtr = bit->second;
			if(bonePtr)
			{
				md5WeightPtr->jIndex = bonePtr->index;
			}
			
		}
		weightList.push_back(md5WeightPtr);
	}

}
