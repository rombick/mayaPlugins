
#include "BulgeDeformer.h"

#include <maya\MGlobal.h>
MTypeId Bulge::id(0x24fd1);
MObject Bulge::attrBulgeValue;

Bulge::Bulge() {
};

Bulge::~Bulge() {
}

void* Bulge::creator() {
	return new Bulge;
}


MStatus Bulge::deform(MDataBlock& data,
	MItGeometry& itGeo,
	const MMatrix& localToWorldMatrix,
	unsigned int geomIndex)
{
	MStatus stat;
	MArrayDataHandle hInputArray = data.outputArrayValue(input, &stat);
	stat = hInputArray.jumpToElement(geomIndex);
	MObject meshObj = hInputArray.outputValue().child(inputGeom).asMesh();

	MFnMesh fnMesh(meshObj, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	
	MFloatVectorArray normals;
	fnMesh.getVertexNormals(false, normals);

	float bulge = data.inputValue(attrBulgeValue).asFloat();
	float env = data.inputValue(envelope).asFloat();
	MPoint point;
	float w; // weight value variable to store weight value in the loop
	for (; !itGeo.isDone(); itGeo.next())
	{
		w = weightValue(data, geomIndex, itGeo.index());
		point = itGeo.position(); // could use fnMesh for the same task, but itGeo allows to use deformer membership
		point = point + normals[itGeo.index()] * bulge * w * env;
		itGeo.setPosition(point);
	}
	return MS::kSuccess;
}

MStatus Bulge::initialize() 
{
	MStatus stat;
	MFnNumericAttribute nAttr;

	attrBulgeValue = nAttr.create("bulge", "bulge", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrBulgeValue);
	attributeAffects(attrBulgeValue, outputGeom);

	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer blendMesh weights;");

	return MS::kSuccess;
}