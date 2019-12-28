
#include "blendMeshDeformer.h"

#include <maya\MGlobal.h>
MTypeId BlendMesh::id(0x255be);
MObject BlendMesh::attrBlendMesh;
MObject BlendMesh::attrBlendWeight;

BlendMesh::BlendMesh() {
};

BlendMesh::~BlendMesh() {
}

void* BlendMesh::creator() {
	return new BlendMesh;
}


MStatus BlendMesh::deform(MDataBlock& data,
							MItGeometry& itGeo,
							const MMatrix& localToWorldMatrix,
							unsigned int geomIndex) 
{
	MStatus stat;
	MDataHandle hBlendMesh = data.inputValue(attrBlendMesh, &stat);
	MObject oBlendMesh = hBlendMesh.asMesh();
	if (oBlendMesh.isNull()) {
		return MS::kSuccess;
	}

	MFnMesh fnMesh(oBlendMesh, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	MPointArray blendPoints;
	fnMesh.getPoints(blendPoints);

	float blendWeight = data.inputValue(attrBlendWeight).asFloat();
	float env = data.inputValue(envelope).asFloat();

	MPoint point;
	float w; // weight value variable to store weight value in the loop
	for (; !itGeo.isDone(); itGeo.next())
	{
		point = itGeo.position();
		w = weightValue(data, geomIndex, itGeo.index());
		point = point + (blendPoints[itGeo.index()] - point) * blendWeight * env * w;
		itGeo.setPosition(point);
	}
	return MS::kSuccess;
}

MStatus BlendMesh::initialize() {
	MStatus stat;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	attrBlendMesh = tAttr.create("blendMesh", "blendMesh", MFnData::kMesh);
	addAttribute(attrBlendMesh);
	attributeAffects(attrBlendMesh, outputGeom);

	attrBlendWeight = nAttr.create("blendWeight", "blendWeight", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	addAttribute(attrBlendWeight);
	attributeAffects(attrBlendWeight, outputGeom);

	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer blendMesh weights;");

	return MS::kSuccess;
}