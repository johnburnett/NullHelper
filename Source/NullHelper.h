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

/* TODO:
-Rendering to the proper depth
*/

#pragma once

#pragma warning(disable: 4514) // unreferenced inline function has been removed
#pragma warning(disable: 4710) // function not expanded
#pragma warning(push, 3)
#include "max.h"

#if (MAX_RELEASE >= 9000)
#include "maxheapdirect.h"	//max 9
#else
#include "max_mem.h"		//max 8 and earlier
#endif

#include "iparamm2.h"
#pragma warning(pop)

#include "resource.h"

#define	NULLHELPER_CLASSID		Class_ID(0x160c0362, 0x6b9f2f1b)


TCHAR *GetString(int id);
extern ClassDesc* GetNullHelperDesc();


// Paramblock2 name
enum { nh_params, };
// Paramblock2 parameter list
enum {
	nh_lockxyz,
	nh_xscale, nh_yscale, nh_zscale,
	nh_solid,
	nh_nullshape,
	nh_displaylabel,
	nh_label,
	nh_nonscaling,
	nh_coloraxis,
	nh_displayparent,
	nh_drawlinkline,
	nh_displayname,
	nh_mixwirecolor,
	nh_centerpoint,
/*
	nh_ref_nodes,
	nh_ref_line,
	nh_ref_distance,
	nh_ref_label,
	nh_linklinetype,
	nh_drawdepth,
*/
};

#define NUMSHAPES		8

#define SHAPE_NONE		0
#define SHAPE_CROSS		1
#define SHAPE_JACK		2
#define SHAPE_BOX		3
#define SHAPE_SPHERE	4
#define SHAPE_VECTOR	5
#define SHAPE_TURTLE	6
#define SHAPE_LINE		7

class NullHelperObject: public HelperObject
{
	public:
		IParamBlock2 *pblock2;
		static IObjParam *ip;

		Mesh mesh;

		Point3 wireColor;
		Point3 labelColor;
		Point3 colorX, colorY, colorZ;

		HWND hAboutRollup;

		NullHelperObject();

		void UpdateMesh(TimeValue t, ViewExp *vpt);
		void BuildMesh(TimeValue t, ViewExp *vpt);
		// From BaseObject
		int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt);
		int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);
		CreateMouseCallBack* GetCreateMouseCallBack();
		TCHAR *GetObjectName() { return GetString(IDS_NULL_CLASSNAME); }

		void BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);
		void InvalidateUI();

		// From Object
		ObjectState Eval(TimeValue time);

		void MaybeEnlargeViewportRect(GraphicsWindow *gw, Rect &rect);
		void InitNodeName(TSTR& s) { s = GetString(IDS_NULL_DEFAULTNAME); }
		int UsesWireColor() { return 1; }
		Interval ObjectValidity(TimeValue t);

		// We don't convert to anything
		int CanConvertToType(Class_ID obtype) {return FALSE;}
		Object* ConvertToType(TimeValue t, Class_ID obtype) {assert(0);return NULL;}

		void GetWorldBoundBox(TimeValue t, INode *mat, ViewExp *vpt, Box3& box );
		void GetLocalBoundBox(TimeValue t, INode *mat, ViewExp *vpt, Box3& box );
		int DoOwnSelectHilite()	{ return 1; }

		// Animatable methods
		void DeleteThis() { delete this; }
		Class_ID ClassID() { return NULLHELPER_CLASSID; }
		void GetClassName(TSTR& s) { s = GetString(IDS_NULL_CLASSNAME); }
		int IsKeyable() { return 0;}

		// Direct paramblock access
		int	NumParamBlocks() { return 1; }
		IParamBlock2* GetParamBlock(int i) { return pblock2; }
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock2->ID() == id) ? pblock2 : NULL; }

		int NumSubs() { return 1; }
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);

		// From ref
 		int NumRefs() { return 1; }
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);

#if (MAX_RELEASE >= 9000)	//max 9
		RefTargetHandle		Clone(RemapDir& remap = DefaultRemapDir());
#else						//max 8 and earlier
		RefTargetHandle		Clone(RemapDir& remap = NoRemap());
#endif

		RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget,
		   PartID& partID, RefMessage message );

		void SetDrawColors(INode* inode);
		void SetNodeWireColor(DWORD col);
};

/*===========================================================================*\
 |	Mouse Creation
\*===========================================================================*/

class NullHelperCreateCallBack : public CreateMouseCallBack {
	NullHelperObject *nho;
	IPoint2 sp0;
	Point3 p0;

	public:
		int proc( ViewExp *vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat);
		void SetObj(NullHelperObject *obj) { nho = obj; }
};


/*===========================================================================*\
 |	ParamBlock Accessor
\*===========================================================================*/

class NullHelperPBAccessor : public PBAccessor
{
	public:
		void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t);
};


/*===========================================================================*\
 |	Dialog Processor
\*===========================================================================*/

class NullHelperDlgProc : public ParamMap2UserDlgProc
{
	public:
		NullHelperObject *nho;

		NullHelperDlgProc() {}
		NullHelperDlgProc(NullHelperObject *nho_in) { nho = nho_in; }

#if (MAX_RELEASE >= 9000)
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#else
		BOOL DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
		void DeleteThis() { }

		void SetThing(ReferenceTarget *m) {
			nho = (NullHelperObject*)m;
		}

		void TwiddleEnables(TimeValue t, HWND hWnd);
};
