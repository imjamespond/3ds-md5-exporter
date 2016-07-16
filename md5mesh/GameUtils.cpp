#include "MaxUtils.h"

void MaxUtils::gameDumpMeshes(IGameScene* pIgame) 
{
	//_MeshCount=0;
	const int NodeCount = pIgame->GetTopLevelNodeCount();
	for(int loop = 0; loop <NodeCount;loop++)
	{
		IGameNode * pGameNode = pIgame->GetTopLevelNode(loop);
		gameDumpMesh(pGameNode);

	}
}

void 
	MaxUtils::gameDumpMesh( IGameNode * pGameNode ) 
{
	IGameObject * obj = pGameNode->GetIGameObject();
	if (!pGameNode->IsNodeHidden()&&
		obj->GetIGameType()==IGameObject::IGAME_MESH)
	{
		IGameMesh * gM = (IGameMesh*)obj;
		if(gM->InitializeData())
		{
			bool hasSkin=false;
			int numMod = obj->GetNumModifiers();
			if(numMod > 0)
			{
				int i=0;
				for(i=0;i<numMod;i++)
				{
					IGameModifier * gMod = obj->GetIGameModifier(i);
					if (gMod->IsSkin())
					{
						hasSkin=true;
						myUtils->writeFun(0,"hasSkin\n");
						MaxUtils::gameExportTriangles(gM);
						//DumpSubMesh(pGameNode,gM,(IGameSkin*)gMod);
						break;
					}
				}
			}
			if (!hasSkin)
			{
				myUtils->writeFun(0,"hasNoSkin\n");
				MaxUtils::gameExportTriangles(gM);
				//DumpSubMesh(pGameNode,gM,NULL);
			}
		}
		else
		{
			myUtils->writeFun(0,"BadObject\n");
		}
	}

	pGameNode->ReleaseIGameObject();

	for(int i=0;i<pGameNode->GetChildCount();i++)
	{
		IGameNode * child = pGameNode->GetNodeChild(i);

		gameDumpMesh(child);
	}
}

void 
	MaxUtils::gameExportTriangles( IGameMesh * gM) 
{
	Tab<int> matIDs=gM->GetActiveMatIDs();
	int matCount=matIDs.Count();
	//hash_map<MCHAR*,vector<FaceEx *> > sameMatMap;

	for (int m=0;m<matCount;++m)
	{
		Tab<FaceEx *> faces=gM->GetFacesFromMatID(matIDs[m]);
		//IGameMaterial * mat=gM->GetMaterialFromFace(faces[0]);
		//vector<FaceEx *>& faceList=sameMatMap[mat->GetMaterialName()];
		for (int f=0;f<faces.Count();++f)
		{
			for (int i=0;i<3;++i)
			{
				DWORD index = faces[f]->vert[i];
				Point3 vertex = gM->GetVertex(index);
				myUtils->writeFun(0,"vertex%d:%f,%f,%f",i,vertex.x,vertex.y,vertex.z);
			}
		}
	}

	//for (hash_map<MCHAR*,vector<FaceEx *> >::iterator it=sameMatMap.begin();
	//	it!=sameMatMap.end();++it)
	//{
	//	DumpFaces(gM,it->second,gSkin,pGameNode,it->first);
	//}
}