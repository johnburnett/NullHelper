/*===========================================================================*\
 |
 |			Generic Null Helper object
 |			3D Studio MAX R3.0
 |
 |  AUTH:   John Burnett
 |
 |  HIST:	Started 5.6.2000
 |
\*===========================================================================*/

#include "NullHelper.h"
#include "Utils.h"
#include "AboutRollup.h"

IObjParam* NullHelperObject::ip = NULL;

/*===========================================================================*\
 |	Class Descriptor
\*===========================================================================*/

class NullHelperClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic()					{ return TRUE; }
	void *			Create( BOOL loading )		{ return new NullHelperObject; }
	const TCHAR *	ClassName()					{ return GetString(IDS_NULL_CLASSNAME); }
	SClass_ID		SuperClassID()				{ return HELPER_CLASS_ID; }
	Class_ID 		ClassID()					{ return NULLHELPER_CLASSID; }
	const TCHAR* 	Category()					{ return GetString(IDS_CATEGORY);  }

	const TCHAR*	InternalName()				{ return _T("NullHelper"); }
	HINSTANCE		HInstance()					{ return hInstance; }
};

static NullHelperClassDesc NullHelperCD;
ClassDesc* GetNullHelperDesc() {return &NullHelperCD;}


/*===========================================================================*\
 |	PBAccessor
\*===========================================================================*/

void NullHelperPBAccessor::Set(PB2Value& v, ReferenceMaker* owner,
							   ParamID id, int tabIndex, TimeValue t)
{
	switch (id) {
		case (nh_nullshape):
			limit (v.i,0,NUMSHAPES);
			break;
/*
		case (nh_linklinetype):
			limit (v.i,1,2);
			break;
		case (nh_drawdepth):
			limit (v.i,1,3);
			break;
*/
	}
};

static NullHelperPBAccessor NullAccessor;

/*===========================================================================*\
 |	Dialog Handler
\*===========================================================================*/

int ShapeStrings[NUMSHAPES+1] = {
	IDS_NULL_SHAPE_NONE,
	IDS_NULL_SHAPE_CROSS,
	IDS_NULL_SHAPE_JACK,
	IDS_NULL_SHAPE_BOX,
	IDS_NULL_SHAPE_SPHERE,
	IDS_NULL_SHAPE_VECTOR,
	IDS_NULL_SHAPE_TURTLE,
	IDS_NULL_SHAPE_LINE,
};

void NullHelperDlgProc::TwiddleEnables(TimeValue t, HWND hWnd)
{
	int v;
	nho->pblock2->GetValue(nh_lockxyz,t,v,FOREVER);
	EnableWindow(GetDlgItem(hWnd,IDC_NULL_YSCALE_EDIT), (!v));
	EnableWindow(GetDlgItem(hWnd,IDC_NULL_ZSCALE_EDIT), (!v));

	BOOL doSolid, doColor;
	nho->pblock2->GetValue(nh_nullshape,t,v,FOREVER);
	if (v == SHAPE_CROSS || v == SHAPE_JACK)
	{
		if (nho->pblock2->GetInt(nh_solid)) {
			doSolid = TRUE;
			doColor = FALSE;
		} else {
			if (nho->pblock2->GetInt(nh_coloraxis)) {
				doSolid = FALSE;
				doColor = TRUE;
			} else {
				doSolid = TRUE;
				doColor = TRUE;
			}
		}
	} else {
		doSolid = TRUE;
		doColor = FALSE;
	}
	EnableWindow(GetDlgItem(hWnd,IDC_NULL_SOLID), doSolid);
	EnableWindow(GetDlgItem(hWnd,IDC_NULL_COLORAXIS), doColor);
}

#if MAX_VERSION_MAJOR < 9 //Max 9
BOOL NullHelperDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
#else
INT_PTR NullHelperDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
#endif
{
	int id = LOWORD(wParam);
	switch (msg)
	{
		case WM_INITDIALOG: {
			TwiddleEnables(t, hWnd);

			HWND hwndList = GetDlgItem(hWnd, IDC_NULL_NULLSHAPE);
			SendMessage(hwndList, CB_RESETCONTENT, 0L, 0L);
			for (int i = 0; i < NUMSHAPES; i++) {
				SendMessage(hwndList, CB_INSERTSTRING, -1L, (LPARAM)(GetString(ShapeStrings[i])) );
			}
			int v;
			nho->pblock2->GetValue(nh_nullshape,t,v,FOREVER);
			SendMessage(hwndList, CB_SETCURSEL, (WPARAM)v, 0L);
			break;
		}
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			switch (id) {
				case IDC_NULL_NULLSHAPE:
				case IDC_NULL_SOLID:
				case IDC_NULL_COLORAXIS:
				case IDC_NULL_LOCKXYZ:
					TwiddleEnables(t, hWnd);
					break;
				case IDC_NULL_COLOR01: nho->SetNodeWireColor(RGB(153,204,255)); break;
				case IDC_NULL_COLOR02: nho->SetNodeWireColor(RGB(255,  0,  0)); break;
				case IDC_NULL_COLOR03: nho->SetNodeWireColor(RGB(  0,255,  0)); break;
				case IDC_NULL_COLOR04: nho->SetNodeWireColor(RGB(  0,  0,255)); break;
				case IDC_NULL_COLOR05: nho->SetNodeWireColor(RGB(  0,255,255)); break;
				case IDC_NULL_COLOR06: nho->SetNodeWireColor(RGB(255,  0,255)); break;
				case IDC_NULL_COLOR07: nho->SetNodeWireColor(RGB(255,255,  0)); break;
				case IDC_NULL_COLOR08: nho->SetNodeWireColor(RGB(  0,  0,  0)); break;
				case IDC_NULL_COLOR09: nho->SetNodeWireColor(RGB(192, 79, 79)); break;
				case IDC_NULL_COLOR10: nho->SetNodeWireColor(RGB( 80,128, 80)); break;
				case IDC_NULL_COLOR11: nho->SetNodeWireColor(RGB( 96, 96,192)); break;
				case IDC_NULL_COLOR12: nho->SetNodeWireColor(RGB( 50, 50,150)); break;
				case IDC_NULL_COLOR13: nho->SetNodeWireColor(RGB(160, 92,160)); break;
				case IDC_NULL_COLOR14: nho->SetNodeWireColor(RGB(170,140,  0)); break;
			}
			break;
		case WM_PAINT:
			break;
	}
	return FALSE;
}

/*===========================================================================*\
 |	Mouse creation
\*===========================================================================*/

int NullHelperCreateCallBack::proc(ViewExp *vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat ) {
	float r;
	Point3 p1, center;

	if (msg == MOUSE_FREEMOVE) {
		vpt->SnapPreview(m, m, NULL, SNAP_IN_3D);
	}

	if (msg==MOUSE_POINT||msg==MOUSE_MOVE) {
		if (flags & MOUSE_SHIFT)
		{
			mat.NoRot();
		} else {
			Matrix3 cMat;
			vpt->GetConstructionTM(cMat);
			mat = Inverse(cMat);
		}

		switch(point)
		{
			case 0:
			{
				sp0 = m;
				p0 = vpt->SnapPoint(m, m, NULL, SNAP_IN_3D);
				mat.SetTrans(p0);
				break;
			}

			case 1:
			{
				if (flags & MOUSE_CTRL)
				{
					p0 = vpt->SnapPoint(m, m, NULL, SNAP_IN_3D);
					mat.SetTrans(p0);

					if (msg==MOUSE_POINT)
					{
						float s;
						nho->pblock2->GetValue(nh_xscale, 0, s, FOREVER);
						return (s == 0.0f) ? CREATE_ABORT : CREATE_STOP;
					}
				} else {
					p1 = vpt->SnapPoint(m, m, NULL, SNAP_IN_3D);
					r = Length(p1-p0) * 2.f;
					mat.SetTrans(p0);

					nho->pblock2->SetValue(nh_xscale, 0, r);

					if (msg==MOUSE_POINT) {
						float s;
						nho->pblock2->GetValue(nh_xscale, 0, s, FOREVER);
						return (Length(m-sp0)<3 || s == 0.0f)?CREATE_ABORT:CREATE_STOP;
					}
				}
				break;
			}
		}
	} else {
		if (msg == MOUSE_ABORT) return CREATE_ABORT;
	}

	return TRUE;
}

static NullHelperCreateCallBack NullHelperCreateCB;

CreateMouseCallBack* NullHelperObject::GetCreateMouseCallBack()
{
	NullHelperCreateCB.SetObj(this);
	return(&NullHelperCreateCB);
}

/*===========================================================================*\
 |	Paramblock2 Descriptor
\*===========================================================================*/
#if MAX_VERSION_MAJOR < 15	//Max 2013
 #define p_end end
#endif

static ParamBlockDesc2 nh_param_blk ( nh_params, _T("NullHelperParameters"),  0, &NullHelperCD, P_AUTO_CONSTRUCT + P_AUTO_UI, 0,
	//rollout
	IDD_NULL_HELPER, IDS_NULL_HELPER_PARAMETERS, 0, 0, NULL,
	// params
	nh_lockxyz,	_T("lockXYZ"),	TYPE_BOOL,	0,	IDS_NULL_LOCKXYZ,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX,	IDC_NULL_LOCKXYZ,
		p_end,
	nh_xscale,	_T("xScale"),	TYPE_FLOAT,	P_ANIMATABLE,	IDS_NULL_XSCALE,
		p_default,		1.0f,
		p_range, 		0.0f, 999999.0f,
		p_ui,			TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_NULL_XSCALE_EDIT, IDC_NULL_XSCALE_SPIN, 0.1f,
		p_end,
	nh_yscale,	_T("yScale"),	TYPE_FLOAT,	P_ANIMATABLE,	IDS_NULL_YSCALE,
		p_default,		1.0f,
		p_range, 		0.0f, 999999.0f,
		p_ui,			TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_NULL_YSCALE_EDIT, IDC_NULL_YSCALE_SPIN, 0.1f,
		p_end,
	nh_zscale,	_T("zScale"),	TYPE_FLOAT,	P_ANIMATABLE,	IDS_NULL_ZSCALE,
		p_default,		1.0f,
		p_range, 		0.0f, 999999.0f,
		p_ui,			TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_NULL_ZSCALE_EDIT, IDC_NULL_ZSCALE_SPIN, 0.1f,
		p_end,
	nh_solid,	_T("solid"),	TYPE_BOOL,	0,	IDS_NULL_SOLID,
		p_default,		FALSE,
		p_ui,			TYPE_SINGLECHEKBOX,	IDC_NULL_SOLID,
		p_end,
	nh_nullshape,	_T("nullShape"),	TYPE_INT,	0,	IDS_NULL_NULLSHAPE,
		p_ui, 			TYPE_INTLISTBOX,	IDC_NULL_NULLSHAPE,	0,
		p_range, 		0, NUMSHAPES,
		p_default,		2,
		p_accessor,		&NullAccessor,
		p_end,
	nh_displaylabel,	_T("displayLabel"),		TYPE_BOOL,	0,	IDS_NULL_DISPLAYLABEL,
		p_default,	FALSE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_DISPLAYLABEL,
		p_end,
	nh_label,		_T("label"),		TYPE_STRING,	0,		IDS_NULL_LABEL,
		p_default,	_T("Label Text"),
		p_ui,		TYPE_EDITBOX,	IDC_NULL_LABEL,
		p_end,
	nh_nonscaling,		_T("nonScalingDisplay"),	TYPE_BOOL,	0,	IDS_NULL_NONSCALING,
		p_default,	FALSE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NONSCALING,
		p_end,
	nh_coloraxis,		_T("colorAxis"),			TYPE_BOOL,	0,	IDS_NULL_COLORAXIS,
		p_default,	TRUE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_COLORAXIS,
		p_end,
	nh_displayparent,	_T("displayParentName"),	TYPE_BOOL,	0,	IDS_NULL_DISPLAYPARENT,
		p_default,	FALSE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_DISPLAYPARENT,
		p_end,
	nh_drawlinkline,	_T("displayLinkLine"),	TYPE_BOOL,	0,	IDS_NULL_DRAWLINKLINE,
		p_default,	FALSE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_DRAWLINKLINE,
		p_end,
	nh_displayname,		_T("displayName"),		TYPE_BOOL,	0,	 IDS_NULL_DISPLAYNAME,
		p_default,	FALSE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_DISPLAYNAME,
		p_end,
	nh_mixwirecolor,	_T("mixWireColor"),	TYPE_BOOL,	0,	IDS_NULL_MIXWIRECOLOR,
		p_default,	TRUE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_MIXWIRECOLOR,
		p_end,
	nh_centerpoint,		_T("displayCenterPoint"),	TYPE_BOOL,	0,	IDS_NULL_CENTERPOINT,
		p_default,	FALSE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_CENTERPOINT,
		p_end,
/*
	nh_ref_nodes,	_T("referenceNodes"), TYPE_INODE_TAB, 0, IDS_NULL_REFERENCENODES,
		p_ui,			TYPE_NODELISTBOX, IDC_NULL_REF_LIST, IDC_NULL_REF_ADD, 0, IDC_NULL_REF_REMOVE,
		p_end,
	nh_ref_line,	_T("displayReferenceLines"), TYPE_BOOL, 0, IDS_NULL_REF_LINE,
		p_default,	TRUE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_REF_LINE,
		p_end,
	nh_ref_distance,	_T("displayReferenceDistances"), TYPE_BOOL, 0, IDS_NULL_REF_DISTANCE,
		p_default,	TRUE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_REF_DISTANCE,
		p_end,
	nh_ref_label,	_T("displayReferenceLabels"), TYPE_BOOL, 0, IDS_NULL_REF_LABEL,
		p_default,	TRUE,
		p_ui,		TYPE_SINGLECHEKBOX,		IDC_NULL_REF_LABEL,
		p_end,
	nh_linklinetype,	_T("linkLineType"),			TYPE_INT,	0,	IDS_NULL_LINKLINETYPE,
		p_ui,		TYPE_RADIO,	2,	IDC_NULL_STRAIGHT,	IDC_NULL_BENT,
		p_vals,		1,	2,
		p_default,	1,
		p_accessor,		&NullAccessor,
		p_end,
	nh_drawdepth,	_T("drawDepth"),	TYPE_INT,	0,	IDS_NULL_DRAWDEPTH,
		p_ui,			TYPE_RADIO, 3,	IDC_NULL_DEPTH_STANDARD,
										IDC_NULL_DEPTH_ONTOP,
										IDC_NULL_DEPTH_ONBOTTOM,
		p_vals,			1,2,3,
		p_accessor,		&NullAccessor,
		p_range, 		1, 3,
		p_default,		1,
		p_end,
*/
	p_end
);

static Point3 black(0.0f,0.0f,0.0f);
static Point3 red(1.0f,0.0f,0.0f);
static Point3 green(0.0f,1.0f,0.0f);
static Point3 blue(0.0f,0.0f,1.0f);
static Point3 hred(0.5f,0.0f,0.0f);
static Point3 hgreen(0.0f,0.5f,0.0f);
static Point3 hblue(0.0f,0.0f,0.5f);

NullHelperObject::NullHelperObject()
{

	pblock2 = NULL;

	wireColor = black;
	colorX = black;
	colorY = black;
	colorZ = black;

	hAboutRollup = NULL;

	NullHelperCD.MakeAutoParamBlocks(this);
	assert(pblock2);
}

void NullHelperObject::InvalidateUI() {
	nh_param_blk.InvalidateUI(pblock2->LastNotifyParamID());
}

void NullHelperObject::BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev ) {
	this->ip = ip;

	NullHelperCD.BeginEditParams(ip, this, flags, prev);
	nh_param_blk.SetUserDlgProc(new NullHelperDlgProc(this));
	hAboutRollup = ip->AddRollupPage(hInstance, MAKEINTRESOURCE(IDD_ABOUT), aboutDlgProc, _T("About"));
}

void NullHelperObject::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next ) {
	NullHelperCD.EndEditParams(ip, this, flags, next);
	ip->DeleteRollupPage(hAboutRollup);

	this->ip = NULL;
}

RefTargetHandle NullHelperObject::Clone(RemapDir& remap) {
	NullHelperObject* newob = new NullHelperObject();
	newob->ReplaceReference(0,pblock2->Clone(remap));
#if MAX_RELEASE > 3100
	BaseClone(this, newob, remap);
#endif
	return(newob);
}


/*===========================================================================*\
 |	Subanim & References support
\*===========================================================================*/

Animatable* NullHelperObject::SubAnim(int i) {
	switch (i) {
		case 0: return pblock2;
		default: return NULL;
	}
}

TSTR NullHelperObject::SubAnimName(int i) {
	switch (i) {
		case 0: return GetString(IDS_NULL_HELPER_PARAMETERS);
		default: return _T("");
	}
}

RefTargetHandle NullHelperObject::GetReference(int i) {
switch (i) {
	case 0: return pblock2;
	default: return NULL;
	}
}

void NullHelperObject::SetReference(int i, RefTargetHandle rtarg) {
	switch (i) {
		case 0: pblock2 = (IParamBlock2*)rtarg; break;
	}
}

#if MAX_VERSION_MAJOR < 17 //Max 2015
RefResult NullHelperObject::NotifyRefChanged(
		Interval changeInt, RefTargetHandle hTarget,
		PartID& partID,  RefMessage message)
#else
RefResult NullHelperObject::NotifyRefChanged(
		const Interval& changeInt, RefTargetHandle hTarget,
		PartID& partID,  RefMessage message, BOOL propagate)
#endif
	{
	switch (message) {
		case REFMSG_CHANGE:
			nh_param_blk.InvalidateUI();
			break;
	}
	return REF_SUCCEED;
}

/*===========================================================================*\
 |	The validity of our parameters
 |	Start at FOREVER, and intersect with the validity of each item
\*===========================================================================*/

Interval NullHelperObject::ObjectValidity(TimeValue t) {
	float f;
	Interval valid = FOREVER;
	pblock2->GetValue(nh_xscale, t, f, valid);
	pblock2->GetValue(nh_yscale, t, f, valid);
	pblock2->GetValue(nh_zscale, t, f, valid);
	return valid;
}

/*===========================================================================*\
 |	Bounding Box calculation
\*===========================================================================*/

void NullHelperObject::GetLocalBoundBox(
		TimeValue t, INode* inode, ViewExp* vpt, Box3& box )
{
#if MAX_VERSION_MAJOR >= 15 //Max 2013
	if ( ! vpt || ! vpt->IsAlive() )
	{
		box.Init();
		return;
	}
#endif
	UpdateMesh(t, vpt);
	if (pblock2->GetInt(nh_nullshape))
		box = mesh.getBoundingBox();
	else
		box = Box3(Point3(-1,-1,-1), Point3(1,1,1));

	if (pblock2->GetInt(nh_drawlinkline))
	{
		INode* parent = inode->GetParentNode();
		if (!parent->IsRootNode())
		{
			Point3 p = (parent->GetNodeTM(t)).GetTrans();
			Matrix3 itm = inode->GetObjectTM(t);
			itm.Invert();
			box += itm * p;
		}
	}
}

void NullHelperObject::GetWorldBoundBox(
		TimeValue t, INode* inode, ViewExp* vpt, Box3& box )
{
#if MAX_VERSION_MAJOR >= 15 //Max 2013
	if ( ! vpt || ! vpt->IsAlive() )
	{
		box.Init();
		return;
	}
#endif
	Matrix3 mat = inode->GetObjectTM(t);
	UpdateMesh(t, vpt);
	if (pblock2->GetInt(nh_nullshape))
		box = mesh.getBoundingBox() * mat;
	else
		box = Box3(Point3(-1,-1,-1), Point3(1,1,1)) * mat;

	if (pblock2->GetInt(nh_drawlinkline))
	{
		INode* parent = inode->GetParentNode();
		if (!parent->IsRootNode())
		{
			box += (parent->GetNodeTM(t)).GetTrans();
		}
	}
}


/*===========================================================================*\
 |	Hit Testing and Object Snapping
\*===========================================================================*/

int NullHelperObject::HitTest(TimeValue t, INode *inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt) {
#if MAX_VERSION_MAJOR >= 15 //Max 2013
	if ( ! vpt || ! vpt->IsAlive() )
	{
		DbgAssert(!_T("Invalid viewport!"));
		return FALSE;
	}
#endif
	BOOL b;
	int i;
	HitRegion hitRegion;
	Interval ivalid;

	// Setup object->screen transform
	Matrix3 m;
	m = inode->GetObjectTM(t);
	GraphicsWindow *gw = vpt->getGW();
	gw->setTransform(m);
	DWORD rlimBackup = gw->getRndLimits();

	// Base rendering level
	pblock2->GetValue(nh_solid,t,b,FOREVER);
	if (b) {
		gw->setRndLimits(GW_WIREFRAME|GW_BACKCULL|GW_PICK);
	} else {
		gw->setRndLimits(GW_WIREFRAME|GW_PICK);
	}

	DWORD rlim = gw->getRndLimits();
	gw->setRndLimits(rlim &= ~GW_Z_BUFFER);

	MakeHitRegion(hitRegion, type, crossing, 4, p);
	gw->setHitRegion(&hitRegion);
	gw->clearHitCode();

	pblock2->GetValue(nh_nullshape,t,i,FOREVER);
	if (i) {
		// Draw mesh
		UpdateMesh(t, vpt);
		mesh.select( gw, NULL, &hitRegion, flags & HIT_ABORTONHIT, 0);
	} else {
		// Draw marker if the null shape is "none"
		gw->marker(&Point3(0,0,0), DIAMOND_MRKR);
	}

	// Draw center point
	if (pblock2->GetInt(nh_centerpoint))
	{
		gw->marker(&Point3(0,0,0), X_MRKR);//DOT_MRKR);
	}

	gw->setRndLimits(rlimBackup);
	return gw->checkHitCode();
}

void NullHelperObject::SetNodeWireColor(DWORD col)
{
	ModContextList list;
	INodeTab NodeTab;

	// Only the selected nodes will be affected
	ip->GetModContexts(list,NodeTab);

	for( int i = 0 ; i < NodeTab.Count() ; i++)
	{
		NodeTab[i]->SetWireColor(col);
	}

	NotifyDependents(FOREVER, PART_DISPLAY, REFMSG_CHANGE);
#if MAX_RELEASE > 3100
	NotifyDependents(FOREVER, 0, REFMSG_NODE_WIRECOLOR_CHANGED);
#endif
	ip->RedrawViews(ip->GetTime());
}

#define MIXAMOUNT	0.75f
#define IMIXAMOUNT	0.25f
void NullHelperObject::SetDrawColors(INode* inode)
{
	DWORD col = inode->GetWireColor();
	wireColor.x = GetRValue(col)/255.0f;
	wireColor.y = GetGValue(col)/255.0f;
	wireColor.z = GetBValue(col)/255.0f;
	labelColor = wireColor;
	colorX = red;
	colorY = green;
	colorZ = blue;

	if (inode->Selected())
	{
		Point3 mixInColor = GetSelColor();
		labelColor = mixInColor;
		if (pblock2->GetInt(nh_mixwirecolor)) {
			mixInColor = mixInColor*MIXAMOUNT;
			wireColor =	mixInColor + wireColor*IMIXAMOUNT;
			colorX =	mixInColor + colorX*IMIXAMOUNT;
			colorY =	mixInColor + colorY*IMIXAMOUNT;
			colorZ =	mixInColor + colorZ*IMIXAMOUNT;
		} else {
			wireColor = mixInColor;
			colorX =	mixInColor;
			colorY =	mixInColor;
			colorZ =	mixInColor;
		}
	} else if (inode->IsFrozen()) {
		Point3 mixInColor = GetFreezeColor();
		labelColor = mixInColor;
		if (pblock2->GetInt(nh_mixwirecolor)) {
			wireColor =	mixInColor + wireColor*IMIXAMOUNT;
			colorX =	mixInColor + colorX*IMIXAMOUNT;
			colorY =	mixInColor + colorY*IMIXAMOUNT;
			colorZ =	mixInColor + colorZ*IMIXAMOUNT;
		} else {
			wireColor = mixInColor;
			colorX =	mixInColor;
			colorY =	mixInColor;
			colorZ =	mixInColor;
		}
	}
}

/*===========================================================================*\
 |	Display the actual helper
\*===========================================================================*/

#define DrawAxis(axis)									\
	gw->polyline(2, line, NULL, NULL, 0, NULL);			\
	if (doJack) {										\
		nd = m.VectorTransform(axis);					\
		d = DotProd(nd, vd);							\
		if (d >= -0.000001f)							\
			gw->marker(p, DOT_MRKR);					\
		else											\
			gw->marker(p, SM_CIRCLE_MRKR);				\
	}

int NullHelperObject::Display(TimeValue t, INode* inode, ViewExp *vpt, int flags) {
#if MAX_VERSION_MAJOR >= 15 //Max 2013
	if ( ! vpt || ! vpt->IsAlive() )
	{
		DbgAssert(!_T("Invalid viewport!"));
		return FALSE;
	}
#endif
	BOOL b;

	// Setup object->screen transform
	Matrix3 m;
	m = inode->GetObjectTM(t);
	GraphicsWindow* gw = vpt->getGW();

	gw->setTransform(m);
	DWORD rlimBackup = gw->getRndLimits();

	// Base rendering level
	pblock2->GetValue(nh_solid,t,b,FOREVER);
	if (b) {
		gw->setRndLimits(GW_WIREFRAME|GW_BACKCULL);
	} else {
		gw->setRndLimits(GW_WIREFRAME);
	}

	// What depth should we draw at
//	pblock2->GetValue(nh_drawdepth,t,i,ivalid);
//	if (i==1)
		// won't draw properly when z-buffer wire is off in Max...?
//		gw->setRndLimits(GW_Z_BUFFER);
//	else {
		DWORD rlim = gw->getRndLimits();
		gw->setRndLimits(rlim &= ~GW_Z_BUFFER);
//	}

	BOOL solid;
	pblock2->GetValue(nh_solid,t,solid,FOREVER);

	SetDrawColors(inode);

	int nullShape;
	pblock2->GetValue(nh_nullshape,t,nullShape,FOREVER);
	if (nullShape)
	{
		BOOL colorAxis;
		pblock2->GetValue(nh_coloraxis,t,colorAxis,FOREVER);
		// if doing coloraxis, otherwise draw dumb mesh
		if (colorAxis && (nullShape == SHAPE_CROSS || nullShape == SHAPE_JACK))
		{
			float sx, sy, sz;
			BOOL lockXYZ;
			BOOL nonScaling;

			pblock2->GetValue(nh_xscale,t,sx,FOREVER);
			pblock2->GetValue(nh_lockxyz,t,lockXYZ,FOREVER);
			sx /= 2.0;
			if (lockXYZ)
			{
				sz = sy = sx;
			} else {
				pblock2->GetValue(nh_yscale,t,sy,FOREVER);
				pblock2->GetValue(nh_zscale,t,sz,FOREVER);
				sy /= 2.0;
				sz /= 2.0;
			}

			pblock2->GetValue(nh_nonscaling,t,nonScaling,FOREVER);
			if (nonScaling) {
				float scaleFactor = vpt->NonScalingObjectSize() *
									vpt->GetVPWorldWidth(Point3(0.f,0.f,0.f)) * 0.01f;
				sx *= scaleFactor;
				sy *= scaleFactor;
				sz *= scaleFactor;
			}

			BOOL doJack = (nullShape == SHAPE_JACK);

			Point3 line[3];
			line[0] = Point3(0.0,0.0,0.0);
			line[1] = Point3(0.0,0.0,0.0);
			Point3* p = &line[1];

			Point3 nd, vd;
			Matrix3 vTM, coordSysTM;
			nd = m.VectorTransform(nd.ZAxis);
			vpt->GetAffineTM(vTM);
			coordSysTM = Inverse(vTM);
			vd = coordSysTM.GetRow(2);
			float d;

			// draw crosshair
			(*p).x = sx;
				gw->setColor(LINE_COLOR, colorX);
				DrawAxis(nd.XAxis)
			(*p).x = -sx;
				gw->setColor(LINE_COLOR, (colorX/2.0f));
				DrawAxis(-nd.XAxis)

			(*p).x = 0.0; (*p).y = sy;
				gw->setColor(LINE_COLOR, colorY);
				DrawAxis(nd.YAxis)
			(*p).y = -sy;
				gw->setColor(LINE_COLOR, (colorY/2.0f));
				DrawAxis(-nd.YAxis)

			(*p).y = 0.0; (*p).z = sz;
				gw->setColor(LINE_COLOR, colorZ);
				DrawAxis(nd.ZAxis)
			(*p).z = -sz;
				gw->setColor(LINE_COLOR, (colorZ/2.0f));
				DrawAxis(-nd.ZAxis)

			gw->setColor(LINE_COLOR, wireColor);
		} else {
			gw->setColor(LINE_COLOR, wireColor);

			// Draw mesh
			UpdateMesh(t, vpt);
			Material mat; // unused, really.........................
			mesh.render(gw, &mat, NULL, COMP_ALL, 1);
		}
	} else {
		gw->setColor(LINE_COLOR, wireColor);

		// Draw marker if the null shape is "none"
		if (solid)
		{
			Point3 nd, vd;
			Matrix3 vTM, coordSysTM;

			nd = m.VectorTransform(nd.ZAxis);

			vpt->GetAffineTM(vTM);
			coordSysTM = Inverse(vTM);
			vd = coordSysTM.GetRow(2);

			float d = DotProd(nd,vd);
			if (d >= -0.000001f)
				gw->marker(&Point3(0,0,0), DOT_MRKR);
			else
				gw->marker(&Point3(0,0,0), SM_CIRCLE_MRKR);
		} else {
			gw->marker(&Point3(0,0,0), DOT_MRKR);
		}
	}

	// Draw center point
	if (pblock2->GetInt(nh_centerpoint))
	{
		gw->setColor(LINE_COLOR, wireColor);

		if (solid)
		{
			Point3 nd, vd;
			Matrix3 vTM, coordSysTM;

			nd = m.VectorTransform(nd.ZAxis);

			vpt->GetAffineTM(vTM);
			coordSysTM = Inverse(vTM);
			vd = coordSysTM.GetRow(2);

			float d = DotProd(nd,vd);
			if (d >= -0.000001f)
				gw->marker(&Point3(0,0,0), DOT_MRKR);
			else
				gw->marker(&Point3(0,0,0), SM_CIRCLE_MRKR);
		} else
			gw->marker(&Point3(0,0,0), DOT_MRKR);
	}

	gw->setColor(TEXT_COLOR, labelColor);

	IPoint3 op;
	gw->hTransPoint(&(Point3 (0.0f,0.0f,0.0f)), &op);
	op.x += 3;

	// Draw label text
	if (pblock2->GetInt(nh_displaylabel))
	{
		op.y -= 16;

#if MAX_VERSION_MAJOR < 12 //Max 2010
		TCHAR* labelText;
#else
		const TCHAR* labelText;
#endif
		pblock2->GetValue(nh_label,t,labelText,FOREVER);
#if MAX_VERSION_MAJOR < 12 //Max 2010
		gw->hText(&op, labelText);
#else
		TSTR labelTextStr = labelText;
		gw->hText(&op, labelTextStr);
#endif

		op.y += 16;
	}

	TSTR nodeText;
	BOOL doName;
	// Draw node name
	pblock2->GetValue(nh_displayname,t,doName,FOREVER);
	if (doName)
	{
		op.y += 1;

		nodeText = _T("N: ");
		nodeText += inode->GetName();

		gw->hText(&op, nodeText);
	}

	// Draw parent name
	if (pblock2->GetInt(nh_displayparent))
	{
		if (doName)
			op.y += 16;
		else
			op.y += 1;

		nodeText = _T("P: ");

		INode* parent = inode->GetParentNode();
		if (!parent->IsRootNode())
			nodeText += parent->GetName();
		else
			nodeText += _T("<none>");

		gw->hText(&op, nodeText);
	}

	// Draw link line
	if (pblock2->GetInt(nh_drawlinkline))
	{
		INode* parent = inode->GetParentNode();
		if (!parent->IsRootNode())
		{
			gw->setTransform(Matrix3(TRUE));

			Point3 line[3];
			line[0] = (inode->GetObjectTM(t)).GetRow(3);
			line[1] = (parent->GetObjectTM(t)).GetRow(3);

			gw->polyline(2, line, NULL, NULL, FALSE, NULL);
			gw->marker(&line[0], DOT_MRKR);
			gw->marker(&line[1], DOT_MRKR);
		}
	}
/*
	if (pblock2->GetInt(nh_ref_line) || pblock2->GetInt(nh_ref_distance))
	{
		int cnt = pblock2->Count(nh_ref_nodes);
		for (int i=0; i<cnt; i++)
		{
			INode* inode = pblock2->GetINode(nh_ref_nodes, t, i);
		}
	}
*/
	gw->setRndLimits(rlimBackup);
	return(0);
}

void NullHelperObject::MaybeEnlargeViewportRect(GraphicsWindow *gw, Rect &rect)
{
	if (pblock2->GetInt(nh_displayname) || pblock2->GetInt(nh_displayparent))
	{
		//TODO: not sure how to avoid enlarging the rect so much since
		//      it relies on node properties which aren't passed in here...?
		rect.right = gw->getWinSizeX();
		rect.bottom = gw->getWinSizeY();
		rect.top -= 30;
	}// else
	if (pblock2->GetInt(nh_displaylabel))
	{
		if (pblock2->GetInt(nh_drawlinkline))
		{
			rect.right = gw->getWinSizeX();
			rect.bottom = gw->getWinSizeY();
		} else {
#if MAX_VERSION_MAJOR < 12 //Max 2010
			TCHAR* labelText;
#else
			const TCHAR* labelText;
#endif
			SIZE size;
			pblock2->GetValue(nh_label,0,labelText,FOREVER);

#if MAX_VERSION_MAJOR < 12 //Max 2010
			gw->getTextExtents(labelText, &size);
#else
			TSTR labelTextStr = labelText;
			gw->getTextExtents(labelTextStr, &size);
#endif
			int hw = rect.w() / 2;
			if (hw < size.cx)
				rect.SetW(hw + size.cx + 3);
			rect.SetY(rect.y() - size.cx - 1);
			rect.SetH(rect.h() + size.cy);
		}
	}
}

ObjectState NullHelperObject::Eval(TimeValue time) {
	return ObjectState(this);
}
