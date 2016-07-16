#include "MaxUtils.h"
#include <string>
#include <utility>      // std::pair
MaxUtils::MaxUtils():
	pMd5Mesh_(nullptr),
	pMd5Anim_(nullptr),
	meshMap()
{

}


int
	MaxUtils::calcNumDescendants(INode* node)
{
	int i;
	int num = 1;	
	for (i=0; i<node->NumberOfChildren(); i++) 
	{
		num += calcNumDescendants(node->GetChildNode(i));
	}

	return num;
}

void
	MaxUtils::exportNodes(INode* node, MyUtils& mu)
{
	int i;	
	for (i=0; i<node->NumberOfChildren(); i++) 
	{
		std::string str = MyUtils::widechar2str(node->GetName());
		mu.writeFun(1, str.c_str());
		exportNodes(node->GetChildNode(i), mu);
	}

}

void
	MaxUtils::dump()
{
	INode *root = gInterface->GetRootNode();

	//DebugPrint(_T("exportTriangles...\n"));
	updateProgress("exportTriangles");
	exportTriangles(root);

	//DebugPrint(_T("exportSkeletons...\n"));
	updateProgress("exportSkeletons");
	addBaseJoint();
	updateProgress("exportSkeletons");
	exportSkeletons(root);
	updateProgress("convertToMd5Mesh");
	convertToMd5Mesh();

	updateProgress("doExport");
	pMd5Mesh_->doExport();
}

int
	MaxData::Mesh::addMaxVertexList(int index, Point3* pv, Point3* puv, Point3* pn)
{
	max_vertex_ptr p(new max_vertex);

	if(pv){
		p->x = pv->x;
		p->y = pv->y;
		p->z = pv->z;
	}

	if(puv){
		p->u = puv->x;
		p->v = 1.0-puv->y;
	}

	if(pn){
		p->nx = pn->x;
		p->ny = pn->y;
		p->nz = pn->z;
	}

	//查询重复的顶点
	std::pair<MaxData::max_vertex_map::iterator, MaxData::max_vertex_map::iterator> ret;
    ret = maxVertexMap.equal_range(index);
    for (MaxData::max_vertex_map::iterator it=ret.first; it!=ret.second; ++it)
	{
		max_vertex_ptr comp = it->second;
		if((comp->u == p->u) && (comp->v == p->v))
		{
			//DebugPrint(_T("duplicate vertex:%d, u:%f, v:%f\n"), index, p->u, p->v);
			return comp->index;//返回原来的索引
		}
	}
	p->index = static_cast<int>(maxVertexMap.size());
	maxVertexMap.insert(std::pair<int, max_vertex_ptr>(index,p));
	
	return p->index;
}

void
	MaxData::Mesh::addMaxIndexList(int index, int v1, int v2, int v3)
{
	max_index_ptr p(new max_index);

	p->index = index;
	p->v1 = v1;
	p->v2 = v2;
	p->v3 = v3;

	maxIndexList.push_back(p);
}

//Matrix3 
//	MaxUtils::toRightHand(Matrix3& mat)
//{
//	Matrix3 md5mat;
//	md5mat.Zero();
//
//	//convert the matrix to a righthanded one, by flipping its Z axis if it's lefthanded
//	if(DotProd (CrossProd( mat.GetRow(0).Normalize(), mat.GetRow(1).Normalize()) , mat.GetRow(2).Normalize()) > 0)
//	{
//		//md5mat = copy mat
//	}
//	else
//	{
//		//md5mat = copy mat
//		md5mat += mat;
//		Point3 row3 = -mat.GetRow(2);
//		md5mat.SetRow(2,row3);
//	}
//	return md5mat;
//}

GMatrix MaxUtils::toRightHand(Matrix3& mat)
{
	GMatrix gmat(mat);
	Point3 pos=gmat.Translation();
	Quat quat=gmat.Rotation();
	Matrix3 ret=gmat.ExtractMatrix3();
	//检测是否是镜像
	bool isMirrored=DotProd( CrossProd( ret.GetRow(0).Normalize(), ret.GetRow(1).Normalize() ).Normalize(), ret.GetRow(2).Normalize() ) < 0;
	//如果是则对镜像进行旋转的修正
	//修正方式还有待调整
	if (isMirrored)
	{
		float tmp;
		tmp=quat.x;
		quat.x=-quat.y;
		quat.y=tmp;
		tmp=quat.z;
		quat.z=quat.w;
		quat.w=-tmp;
	}
	ret.SetRotate(quat);
	ret.SetTrans(pos);
	return GMatrix(ret);
}