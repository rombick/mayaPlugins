
#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

#include "aimConstraintNode.h"


MStatus initializePlugin(MObject obj) {
	MStatus	stat;
	MFnPlugin plugin(obj, "Roman Rajbhandari", "Any");

	stat = plugin.registerNode("aimConstraintNode",
		AimConstraint::id,
		AimConstraint::creator,
		AimConstraint::initialize);
	CHECK_MSTATUS_AND_RETURN_IT(stat)

		return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
	MStatus	stat;
	MFnPlugin plugin(obj);

	stat = plugin.deregisterNode(AimConstraint::id);
	CHECK_MSTATUS_AND_RETURN_IT(stat);


	return MS::kSuccess;
}