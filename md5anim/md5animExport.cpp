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
#include <boost/lexical_cast.hpp>
#include "md5animExport.h"
#include "MaxUtils.h"
#include <Commctrl.h>
#define md5anim_CLASS_ID	Class_ID(0x6fdbea13, 0x83f32a8e)

class md5anim : public SceneExport {
public:
	//Constructor/Destructor
	md5anim();
	~md5anim();

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



class md5animClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new md5anim(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return md5anim_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("md5anim"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* Getmd5animDesc() { 
	static md5animClassDesc md5animDesc;
	return &md5animDesc; 
}


INT_PTR CALLBACK md5animOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static md5anim* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (md5anim *)lParam;
			imp->setOption(hWnd);
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;

		case WM_COMMAND:
			switch (wParam) {
				case IDC_EXPORT_BTN:
					imp = (md5anim *)lParam;
					imp->onExport(hWnd);
					break;
			}
			return TRUE;

		case WM_NOTIFY:
			return TRUE;

		case WM_MOVE:
			return TRUE;

		case PBM_SETPOS:
			return TRUE;
	}
	return 0;
}


//--- md5anim -------------------------------------------------------
md5anim::md5anim()
{

}

md5anim::~md5anim() 
{

}

int md5anim::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *md5anim::Ext(int /*i*/)
{		
	//#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("md5anim");
}

const TCHAR *md5anim::LongDesc()
{
	//#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("a pluging for 3ds max export md5 animation");
}
	
const TCHAR *md5anim::ShortDesc() 
{			
	//#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("md5 anim export");
}

const TCHAR *md5anim::AuthorName()
{			
	//#pragma message(TODO("Return ASCII Author name"))
	return _T("James Huang");
}

const TCHAR *md5anim::CopyrightMessage() 
{	
	//#pragma message(TODO("Return ASCII Copyright message"))
	return _T("use by your own reason");
}

const TCHAR *md5anim::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *md5anim::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("0.1");
}

unsigned int md5anim::Version()
{				
	//#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void md5anim::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

BOOL md5anim::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	//#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}

Interface* gInterface;
MyUtils *pMyUtils=nullptr;
MaxUtils *pMaxUtils=nullptr;
void init();
int	md5anim::DoExport(const TCHAR* name/**/, ExpInterface* /*ei*/, Interface* ip/**/, BOOL suppressPrompts, DWORD /*options*/)
{
	//#pragma message(TODO("Implement the actual file Export here and"))
	//save interface for glabal use
	gInterface = ip;

	Md5Anim md5Anim;
	MyUtils myUtils(name);
	MaxUtils maxUtils;
	pMyUtils = &myUtils;
	pMaxUtils = &maxUtils;
	maxUtils.setMd5Anim(&md5Anim);
	pMaxUtils = &maxUtils;

	init();

	if(!suppressPrompts)
	{
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				md5animOptionsDlgProc, (LPARAM)this);
	}

	//#pragma message(TODO("return TRUE If the file is exported properly"))
	return TRUE;
}

int gFrameStart(0);
int gFrameEnd(0);
float gBJX(0.0);
float gBJY(0.0);
float gBJZ(0.0);
void init()
{
	int tpf = GetTicksPerFrame();
	gFrameStart = gInterface->GetAnimRange().Start()/tpf,
    gFrameEnd = gInterface->GetAnimRange().End()/tpf;
}

int gRadios[] = {IDC_RADIO1, IDC_RADIO2, IDC_RADIO3, IDC_RADIO4, IDC_RADIO5, IDC_RADIO6};
int gFlagRadios[] = {IDC_RADIO7, IDC_RADIO8};
void md5anim::setOption(HWND hWnd)
{
	SetDlgItemInt(hWnd, IDC_FRAME_START, static_cast<UINT>(gFrameStart), false);
	SetDlgItemInt(hWnd, IDC_FRAME_END, static_cast<UINT>(gFrameEnd), false);
	SetDlgItemInt(hWnd, IDC_BJ_INC1, static_cast<UINT>(0), false);
	SetDlgItemInt(hWnd, IDC_BJ_INC2, static_cast<UINT>(0), false);
	SetDlgItemInt(hWnd, IDC_BJ_INC3, static_cast<UINT>(0), false);


	//set default and ratio check,Only the first radio button in the group should have the Group property set to True
	CheckRadioButton(hWnd,gRadios[0], gRadios[5], gRadios[4]);
	CheckRadioButton(hWnd,gFlagRadios[0], gFlagRadios[1], gFlagRadios[0]);
}
void initProgress();
float flagPrecision(.01f);
void md5anim::onExport(HWND hWnd)
{
	BOOL success = false;
	gFrameStart = GetDlgItemInt(hWnd, IDC_FRAME_START, &success, false);
	//DebugPrint(_T("gFrameStart:%d,%d\n"),gFrameStart,success);
	success = false;
	gFrameEnd = GetDlgItemInt(hWnd, IDC_FRAME_END, &success, false);
	//DebugPrint(_T("gFrameEnd:%d,%d\n"),gFrameEnd,success);
	wchar_t text[20];
	std::string str;
	::memset(text, 0, (sizeof text)/sizeof(wchar_t));
	GetDlgItemText(hWnd, IDC_BJ_INC1, text, 20);
	str = MyUtils::widechar2str(text);
	::memset(text, 0, (sizeof text)/sizeof(wchar_t));
	gBJX = boost::lexical_cast<float, std::string>(str);
	GetDlgItemText(hWnd, IDC_BJ_INC2, text, 20);
	str = MyUtils::widechar2str(text);
	gBJY = boost::lexical_cast<float, std::string>(str);
	::memset(text, 0, (sizeof text)/sizeof(wchar_t));
	GetDlgItemText(hWnd, IDC_BJ_INC3, text, 20);
	str = MyUtils::widechar2str(text);
	gBJZ = boost::lexical_cast<float, std::string>(str);

	for(int i(0);i<sizeof gRadios/sizeof(int);i++)
	{
		if( IsDlgButtonChecked(hWnd,gRadios[i])){
			Md5Anim::setPrecision(i);
			break;
		}
	}
	flagPrecision = 0.001f;
	for(int i(0);i<sizeof gFlagRadios/sizeof(int);i++)
	{
		if( IsDlgButtonChecked(hWnd,gFlagRadios[i])){
			break;
		}
		flagPrecision*=0.01f;
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
