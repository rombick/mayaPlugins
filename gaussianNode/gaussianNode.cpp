
#include "gaussianNode.h"

#include <maya\MGlobal.h>
#include <math.h>

MTypeId GaussianNode::id(0x00000231);

MObject GaussianNode::attrOutValue;
MObject GaussianNode::attrInValue;
MObject GaussianNode::attrMagnitute;
MObject GaussianNode::attrMean;
MObject GaussianNode::attrVariance;

GaussianNode::GaussianNode(){
};

GaussianNode::~GaussianNode(){
}

void* GaussianNode::creator() {
	return new GaussianNode();
}

MStatus GaussianNode::compute( const MPlug& plug, MDataBlock& data) {
	MStatus stat;

	if (plug != attrOutValue) {
		return MS::kUnknownParameter;
	}
	float inputValue = data.inputValue(attrInValue, &stat).asFloat();
	float magnitude = data.inputValue(attrMagnitute, &stat).asFloat();
	float mean = data.inputValue(attrMean, &stat).asFloat();
	float variance = data.inputValue(attrVariance, &stat).asFloat();
	
	if (variance <= 0.0f) {
		variance = 0.001f;
	}

	float xMinusB = inputValue - mean;
	float output = magnitude * exp(-(xMinusB*xMinusB)/(2.0f * variance));

	MDataHandle handleOutput = data.outputValue(attrOutValue, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	handleOutput.setFloat(output);
	handleOutput.setClean();

	return MS::kSuccess;
}

MStatus GaussianNode::initialize() {
	MStatus stat;
	MFnNumericAttribute nAttr;

	attrOutValue = nAttr.create("outValue", "outValue", MFnNumericData::kFloat);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutValue);

	attrInValue = nAttr.create("inValue", "inValue", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrInValue);
	attributeAffects(attrInValue, attrOutValue);

	attrMagnitute = nAttr.create("magnitude", "magnitude", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrMagnitute);
	attributeAffects(attrMagnitute, attrOutValue);

	attrMean = nAttr.create("mean", "mean", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrMean);
	attributeAffects(attrMean, attrOutValue);

	attrVariance = nAttr.create("variance", "variance", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrVariance);
	attributeAffects(attrVariance, attrOutValue);

	return MS::kSuccess;

}