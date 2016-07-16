#include "MaxUtils.h"
#include <MeshNormalSpec.h>

// evaluate the State the object after offset and WSM's applied		
// if evalHidden is FALSE and the node is hidden the pipeline will not
// actually be evaluated (however the TM will).
/*! \remarks This method should be called when a developer needs to
work with an object that is the result of the node's pipeline. This is
the object that the appears in the scene.\n\n
This may not be an object that anyone has a reference to - it may just
be an object that has flowed down the pipeline. For example, if there
is a Sphere in the scene that has a Bend and Taper applied,
<b>EvalWorldState()</b> would return an <b>ObjectState</b> containing a
TriObject. This is the result of the sphere turning into a TriObject
and being bent and tapered (just as it appeared in the scene).\n\n
!!!!!!!!!If a developer needs to access the object that the node in the scene
references!!!!!!!!!!, then the method <b>INode::GetObjectRef()</b> should be used
instead.
\param time Specifies the time to retrieve the object state.
\param evalHidden If FALSE and the node is hidden, the pipeline will not actually be
evaluated (however the TM will).
\return  The ObjectState that is the result of the pipeline. See
Class ObjectState.
\code
// Get the object from the node
ObjectState os = node->EvalWorldState(ip->GetTime());

if (os.obj->SuperClassID()==GEOMOBJECT_CLASS_ID)
{
obj = (GeomObject*)os.obj;
//...
}
\endcode
The following code shows how a TriObject can be retrieved from a node. Note on the code that if you call ConvertToType() on an object and it returns a pointer other than itself, you are responsible for deleting that object.
\code
// Retrieve the TriObject from the node
int deleteIt;
TriObject *triObject = GetTriObjectFromNode(ip->GetSelNode(0), deleteIt);

// Use the TriObject if available
if (!triObject) return;
// ...
// Delete it when done...
if (deleteIt) triObject->DeleteMe();

// Return a pointer to a TriObject given an INode or return NULL
// if the node cannot be converted to a TriObject
TriObject *Utility::GetTriObjectFromNode(INode *node, int &deleteIt)
{
deleteIt = FALSE;
Object *obj = node->EvalWorldState(ip->GetTime()).obj;

if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
{
TriObject *tri = (TriObject *) obj->ConvertToType(ip->GetTime(),
Class_ID(TRIOBJ_CLASS_ID, 0));
// Note that the TriObject should only be deleted
// if the pointer to it is not equal to the object
// pointer that called ConvertToType()
if (obj != tri) deleteIt = TRUE;
return tri;
}
else
{
return NULL;
}
}
\endcode
*/
void
	MaxUtils::exportTriangles(INode* node){

		Object *obj = node->EvalWorldState(0).obj;
		if(obj){
			//myUtils->writeFun(1,"node:%s, a:%d, b:%d",MyUtils::widechar2str(node->GetName()).c_str(), static_cast<int>(obj->ClassID().PartA()), static_cast<int>(obj->ClassID().PartB()));
			//myUtils->writeFun(1,"node:%s, a:%x, b:%x",MyUtils::widechar2str(node->GetName()).c_str(), obj->ClassID().PartA(), obj->ClassID().PartB());
		}

		if (obj && obj->ClassID() == BONE_OBJ_CLASSID) {
		}else if (obj && obj->ClassID() == SKELOBJ_CLASS_ID) {
		}else if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {

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
				ISkin *skin(nullptr);
				if (derivedObject && skinIdx >= 0) 
				{
					//取得修改器
					Modifier *mod = derivedObject->GetModifier(skinIdx);
					skin = (ISkin *)mod->GetInterface(I_SKIN);

					if(skin && skin->GetNumBones())
					{
						parseTriangle(node);
					}
				}
			}

		}else if(obj && static_cast<int>(obj->ClassID().PartA()) == -464580429) {
			//bip类型骨骼
			//parseTriangle(node);
		}

		int i;	
		for (i=0; i<node->NumberOfChildren(); i++) 
		{
			exportTriangles(node->GetChildNode(i));
		}
}

void 
	MaxUtils::parseTriangle(INode* node)
{
	MaxData::max_mesh_ptr maxMeshPtr(new MaxData::max_mesh);
	//Mesh names
	maxMeshPtr->name = MyUtils::widechar2str(node->GetName());
	meshMap[maxMeshPtr->name] = maxMeshPtr;
	//Material
	Mtl* mtl = node->GetMtl();
	if(mtl){
		maxMeshPtr->shaderName = MyUtils::widechar2str(mtl->GetName());
	}

	//This method returns the world space transformation matrix of the node at the specified time. 
	Matrix3 matrix = node->GetObjectTM(0);	

	// Retrieve the TriObject from the node
	int deleteIt = FALSE;
	TriObject *triObject = nullptr;//GetTriObjectFromNode(node, deleteIt);
	//GetTriObjectFromNode
	Object *obj = node->EvalWorldState(0).obj;
	if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		triObject = (TriObject *) obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != triObject) deleteIt = TRUE;
	}

	// Use the TriObject if available
	if (!triObject) return;

	Mesh& mesh = triObject->GetMesh();
	MeshNormalSpec *normals = mesh.GetSpecifiedNormals();

	//网格模型的三角形数量
	int numTris = mesh.getNumFaces();
	for (int t=0; t<numTris; t++) 
	{

		//char faceStr[15];
		//sprintf(faceStr, "Face:%d ",t);
		//myUtils->writeFun(1, faceStr);

		int indices[3] = {0,0,0};

		//每个面的三个顶点
		Face face = mesh.faces[t];
		for(int i=0;i<3;++i)
		{
			DWORD vIdx = face.getVert(i);
			Point3& vtx = mesh.getVert(vIdx);
			Point3 wvtx = matrix.PointTransform(vtx);

			Point3* uv = nullptr;
			Point3* normal = nullptr;
			// Might not have UV coords
			if (mesh.tvFace) 
			{
				TVFace tvface = mesh.tvFace[t];
				int tvIdx = tvface.getTVert(i);
				uv = &(mesh.getTVert(tvIdx));
			}

			// Might not have normals
			if (normals && normals->GetNumNormals()>0) 
			{
				normal = &(normals->GetNormal(t, i));
			}

			//导入[顶点索引] 顶点,UV,法线
			indices[i] = maxMeshPtr->addMaxVertexList(vIdx, &wvtx, uv, normal);
		}
		//左手或右手旋转(2,1,0或0,1,2)决定纹理显示方向
		maxMeshPtr->addMaxIndexList(t, indices[0], indices[2], indices[1]);
	}

	//mesh.FreeAll();//not neccesary to free a reference
	triObject->FreeCaches();

	// ...
	// Delete it when done...
	if (deleteIt) triObject->DeleteMe();
	// Return a pointer to a TriObject given an INode or return NULL
	// if the node cannot be converted to a TriObject

}