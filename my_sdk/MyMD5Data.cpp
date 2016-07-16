#include "MyMD5Data.h"

//骨头排序按层级 以广度优先 按名字顺序
void BoneSort(vector<BoneInfo>& boneList)
{
	vector<BoneInfo> tmpList(boneList.begin(),boneList.end());
	boneList.clear();
	vector<BoneInfo>::iterator it=tmpList.begin();

	boneList.push_back(*it);
	tmpList.erase(it);
	
	int parentIndex=0;
	while (!tmpList.empty())
	{
		BoneInfo& parent=boneList.at(parentIndex);
		vector<BoneInfo>::iterator childIt=tmpList.begin();
		vector<BoneInfo> children;
		while (childIt!=tmpList.end())
		{
			if (childIt->ParentIndex==parent.SelfIndex)
			{
				children.push_back(*childIt);
				tmpList.erase(childIt);
				childIt=tmpList.begin();
			}
			else
				++childIt;
		}
		sort(children.begin(),children.end());
		boneList.insert(boneList.end(),children.begin(),children.end());
		++parentIndex;
	}
}
