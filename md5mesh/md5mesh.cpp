//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "md5mesh.h"
#include "maxutils.h"

#define md5mesh_CLASS_ID	Class_ID(0x838531b9, 0x80bc7d02)

class md5mesh : public SceneExport {
public:
	//Constructor/Destructor
	md5mesh();
	~md5mesh();

	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options);
	int  DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	void setOption(HWND hWnd);
	void onExport(HWND hWnd);
};



class md5meshClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new md5mesh(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return md5mesh_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("md5mesh"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* Getmd5meshDesc() { 
	static md5meshClassDesc md5meshDesc;
	return &md5meshDesc; 
}





INT_PTR CALLBACK md5meshOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static md5mesh* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (md5mesh *)lParam;
			imp->setOption(hWnd);
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;

		case WM_COMMAND:
			switch (wParam) {
				case IDC_EXPORT_BTN:
					imp = (md5mesh *)lParam;
					imp->onExport(hWnd);
					break;
			}
			return TRUE;
	}
	return 0;
}


//--- md5mesh -------------------------------------------------------
md5mesh::md5mesh()
{

}

md5mesh::~md5mesh() 
{

}

int md5mesh::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *md5mesh::Ext(int /*i*/)
{		
	return _T("md5mesh");
}

const TCHAR *md5mesh::LongDesc()
{
	return _T("a pluging for 3ds max export md5 mesh");
}
	
const TCHAR *md5mesh::ShortDesc() 
{			
	return _T("md5 mesh export");
}

const TCHAR *md5mesh::AuthorName()
{			
	return _T("James Huang");
}

const TCHAR *md5mesh::CopyrightMessage() 
{	
	return _T("use by your own reason");
}

const TCHAR *md5mesh::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *md5mesh::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int md5mesh::Version()
{				
	//#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void md5mesh::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

BOOL md5mesh::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	//#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}

Interface* gInterface;
MyUtils *pMyUtils=nullptr;
MaxUtils *pMaxUtils=nullptr;
int	md5mesh::DoExport(const TCHAR* name/**/, ExpInterface* /*ei*/, Interface* ip/**/, BOOL suppressPrompts, DWORD /*options*/)
{
	//#pragma message(TODO("Implement the actual file Export here and"))

	//save interface for glabal use
	gInterface = ip;

	Md5Mesh md5Mesh;
	MyUtils myUtils(name);
	MaxUtils maxUtils;
	pMyUtils = &myUtils;
	pMaxUtils = &maxUtils;
	maxUtils.setMd5Mesh(&md5Mesh);

	if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				md5meshOptionsDlgProc, (LPARAM)this);

	//IGameScene* pIgame = GetIGameInterface();
	//pIgame->InitialiseIGame();
	//pIgame->SetStaticFrame(0);
	//maxUtils.gameDumpMeshes(pIgame);
	//pIgame->ReleaseIGame();

	//#pragma message(TODO("return TRUE If the file is exported properly"))
	return TRUE;
}

int gRadios[] = {IDC_RADIO1, IDC_RADIO2, IDC_RADIO3, IDC_RADIO4};
void md5mesh::setOption(HWND hWnd)
{
	SetDlgItemInt(hWnd, IDC_MAX_WEIGHTS, static_cast<UINT>(3), false);
	//set default and ratio check
	CheckRadioButton(hWnd,gRadios[0], gRadios[3], gRadios[2]);
}

int gWeightNum(0);
void initProgress();
void md5mesh::onExport(HWND hWnd)
{
	BOOL success = false;
	gWeightNum = GetDlgItemInt(GetActiveWindow(), IDC_MAX_WEIGHTS, &success, false);

	for(int i(0);i<sizeof gRadios/sizeof(int);i++)
	{
		if( IsDlgButtonChecked(hWnd,gRadios[i])){
			pMaxUtils->getMd5Mesh()->setPrecision(i);
			break;
		}
	}

	initProgress();
	pMaxUtils->dump();
	//DestroyWindow(hwndPB);

	MessageBox( GetActiveWindow(),  _T("all jobs done!"), _T("Message"), 0 );
	EndDialog(GetActiveWindow(), IDOK);
}

void initProgress()
{
	HWND hwndPB = GetDlgItem( GetActiveWindow(), IDC_PROGRESS1 );
	if(hwndPB)
	{
		::ShowWindow(hwndPB,SW_SHOW);
		SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 5));//
		SendMessage(hwndPB, PBM_SETSTEP, (WPARAM) 1, 0); 
	}

	HWND hwndExportBtn = GetDlgItem( GetActiveWindow(), IDC_EXPORT_BTN );
	if(hwndExportBtn)
	{
		::ShowWindow(hwndExportBtn,SW_HIDE);
	}
}

void updateProgress(const char * msg, bool progress)
{
	HWND hwndPB = GetDlgItem( GetActiveWindow(), IDC_PROGRESS1 );
	if(hwndPB&&progress)
	{
		SendMessage(hwndPB, PBM_STEPIT, 0, 0); 
	}
	const wchar_t *wmsg = MyUtils::UTF8ToUnicode(msg);
	SetDlgItemTextW(GetActiveWindow(), IDC_TEXT_PROGRESS, wmsg);
	delete wmsg;
}
