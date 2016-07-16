#include "MaxUtils.h"
#include <string>

void
	MaxUtils::exportSkeletons(INode* node)
{
	Object *obj = node->GetObjectRef();
	INode* pnode = node->GetParentNode();
	Object *pobj = nullptr;
	if(pnode)
	{
		pobj = pnode->GetObjectRef();
	}

	/*if(obj&&pobj)
	{
		DebugPrint(_T("node:%s,pnode:%s, a:0x%10x, a:0x%10x\n"),node->GetName(),pnode->GetName(),  obj->ClassID().PartA(), pobj->ClassID().PartA());
	}*/

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

		//This method returns the transformation matrix which the object needs to be multiplied by to transform it into world space.
		Matrix3 mtx = node->GetObjectTM(0);//GetNodeTM(0);
		GMatrix gmtx = this->toRightHand(mtx);
		Point3 pos = gmtx.Translation();
		bonePtr->id = obj;
		bonePtr->x = pos.x;
		bonePtr->y = pos.y;
		bonePtr->z = pos.z;
		Quat quat = gmtx.Rotation();
		//what for?
		if (quat.w<0)
		{
			quat=-quat;
		}
		bonePtr->ox = quat.x;
		bonePtr->oy = quat.y;
		bonePtr->oz = quat.z;
	}
	else//mesh obj
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
					ISkinContextData * context = skin->GetContextInterface(node);

					//取得顶点数
					//int numVerts = context->GetNumPoints();
					//std::vector<int> tmpIndices;

					DebugPrint(_T("skin:%d, bone num:%d\n"), skinIdx, skin->GetNumBones());

					//遍历每个顶点
					if(meshMap.end() != it)
					{
						MaxData::max_mesh_ptr& meshPtr = it->second;
						MaxData::max_vertex_map_it vit;
						for(vit = meshPtr->maxVertexMap.begin(); vit != meshPtr->maxVertexMap.end(); vit++)
						{
							int iBone;
							int numBones;

							MaxData::max_vertex_ptr vPtr = vit->second;
							int index = vit->first;//原始顶点索引
							//每个顶点受影响的骨骼数
							numBones = context->GetNumAssignedBones(index);
							for (iBone=0; iBone<(numBones); iBone++)//numBones>3?3:numBones
							{
								//取得每个顶点对应的骨骼,通过骨骼计算出初始姿态
								int boneIdx = context->GetAssignedBone(index, iBone);
								INode *bone = skin->GetBone(boneIdx);
								GMatrix gmt;
								if(bone)
								{
									////This method returns the world space transformation matrix of the node at the specified time. 
									Matrix3 mt = bone->GetObjectTM(0);//intial pose
									gmt = GMatrix(mt);
									Point3 t = gmt.Translation();
									Quat qt = gmt.Rotation();
									mt.SetRotate(qt);
									mt.SetRow(3,t);
									gmt = GMatrix(mt);
								}else
								{
									gmt.SetIdentity();
								}
								Point3 vertex(vPtr->x, vPtr->y, vPtr->z);
								gmt=gmt.Inverse();
								vertex=vertex *gmt;

								//顶点对应骨骼取得权重
								float weight = context->GetBoneWeight(index, iBone);

								MaxData::max_weight_ptr wPtr(new MaxData::max_weight);
								vPtr->weightList.push_back(wPtr);
								wPtr->bias = weight;
								wPtr->boneName = MyUtils::widechar2str(bone->GetName());
								//FIXME some transform
								wPtr->wx = vertex.x;
								wPtr->wy = vertex.y;
								wPtr->wz = vertex.z;
							}
						}

					}

				}

			}

		}
	}
	// This isn't a bone, but there might be bones
	// further down the hierarchy from this one
	int i;	
	for (i=0; i<node->NumberOfChildren(); i++) 
	{
		exportSkeletons(node->GetChildNode(i));
	}

}


