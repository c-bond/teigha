#pragma once

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgDatabase.h"
#include "DbDatabase.h"
#include "DynamicLinker.h"
#include "RxInit.h"
#include "RxObjectImpl.h"
#include "RxDynamicModule.h"
#include "diagnostics.h"
#include "DgLevelFilterTableRecord.h"

//#include <experimental/filesystem>

#include "Gi/GiDrawable.h"
#include "RxVariant.h"
#include "DgDatabase.h"
#include "DgLine.h"
#include "DgLineString.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgShape.h"
#include "DgCurve.h"
#include "DgEllipse.h"
#include "DgArc.h"
#include "DgCone.h"
#include "DgComplexString.h"
#include "DgComplexShape.h"
#include "DgPointString.h"
#include "DgDimension.h"
#include "DgMultiline.h"
#include "DgBSplineCurve.h"
#include "Dg3DObject.h"
#include "DgRasterAttach.h"
#include "DgRaster.h"
#include "DgTagElement.h"
#include "DgCellHeader.h"
#include "DgBSplineSurface.h"
#include "DgLevelTableRecord.h"
#include "DgFontTableRecord.h"
#include "DgTextStyleTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include "DgMultilineStyleTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgRegAppTableRecord.h"
#include "DgView.h"
#include "DgSharedCellDefinition.h"
#include "DgSharedCellReference.h"
#include "DgTagSetDefinition.h"
#include "DgReferenceAttach.h"
#include "Ge/GeExtents3d.h"
#include "RxObjectImpl.h"
#include "DgMesh.h"
#include "DgProxyElement.h"
#include "DgApplicationData.h"


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#define  STD(a)  std:: a

/************************************************************************/
/* Define a Custom Services class.                                      */
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};

enum class cad_filetype {unknown, dgn, dwg};
