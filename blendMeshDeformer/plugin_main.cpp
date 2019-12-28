#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

#include "BlendMeshDeformer.h"


MStatus initializePlugin(MObject obj) {
	MFnPlugin plugin(obj, "Roman Rajbhandari", "Any");
	MStatus	stat;
	stat = plugin.registerNode("blendMesh",
		BlendMesh::id,
		BlendMesh::creator,
		BlendMesh::initialize,
		MPxNode::kDeformerNode);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return stat;
}

MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	MStatus	stat;
	stat = plugin.deregisterNode(BlendMesh::id);

	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return stat;
}