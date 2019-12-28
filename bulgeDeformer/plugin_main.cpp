#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

#include "bulgeDeformer.h"


MStatus initializePlugin(MObject obj) {
	MFnPlugin plugin(obj, "Roman Rajbhandari", "Any");
	MStatus	stat;
	stat = plugin.registerNode("bulgeMesh",
		Bulge::id,
		Bulge::creator,
		Bulge::initialize,
		MPxNode::kDeformerNode);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return stat;
}

MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	MStatus	stat;
	stat = plugin.deregisterNode(Bulge::id);

	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return stat;
}