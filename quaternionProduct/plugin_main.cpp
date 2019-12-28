
#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

#include "quaternionProduct.h"


MStatus initializePlugin(MObject obj) {
	MStatus	stat;
	MFnPlugin plugin(obj, "Roman Rajbhandari", "Any");

	stat = plugin.registerNode("quaternionProduct",
		QuaternionProduct::id,
		QuaternionProduct::creator,
		QuaternionProduct::initialize);
	CHECK_MSTATUS_AND_RETURN_IT(stat)

		return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
	MStatus	stat;
	MFnPlugin plugin(obj);

	stat = plugin.deregisterNode(QuaternionProduct::id);
	CHECK_MSTATUS_AND_RETURN_IT(stat);


	return MS::kSuccess;
}