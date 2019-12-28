
#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

#include "gaussianNode.h"


MStatus initializePlugin(MObject obj) {
	MStatus	stat;
	MFnPlugin plugin(obj, "Roman Rajbhandari", "Any");

	stat = plugin.registerNode("gaussian", 
		GaussianNode::id, 
		GaussianNode::creator, 
		GaussianNode::initialize);
	CHECK_MSTATUS_AND_RETURN_IT(stat)

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
	MStatus	stat;
	MFnPlugin plugin(obj);

	stat = plugin.deregisterNode(GaussianNode::id);
	CHECK_MSTATUS_AND_RETURN_IT(stat);


	return MS::kSuccess;
}