#include "quaternionProduct.h"


MTypeId QuaternionProduct::id(0x6d8e8);
MObject QuaternionProduct::attrOutValue;
MObject QuaternionProduct::attrOutValueX;
MObject QuaternionProduct::attrOutValueY;
MObject QuaternionProduct::attrOutValueZ;
MObject QuaternionProduct::attrOutValueW;
MObject QuaternionProduct::attrInValue;
MObject QuaternionProduct::attrInValueX;
MObject QuaternionProduct::attrInValueY;
MObject QuaternionProduct::attrInValueZ;
MObject QuaternionProduct::attrInValueW;


QuaternionProduct::QuaternionProduct() {
};

QuaternionProduct::~QuaternionProduct() {
}

void* QuaternionProduct::creator() {
	return new QuaternionProduct;
}

MStatus QuaternionProduct::compute(const MPlug& plug, MDataBlock& data) {
	MStatus stat;
	int index = 0;

	if (plug != attrOutValue) {
		return MS::kUnknownParameter;
	}
	MArrayDataHandle quatList = data.inputArrayValue(attrInValue);
	unsigned count = quatList.elementCount();
	

	MQuaternion outputQuat = MQuaternion(0, 0, 0, 1);
	for (index = 0; index < count; index++) {
		quatList.jumpToArrayElement(index);
		MQuaternion quatVal = MQuaternion(quatList.inputValue().child(attrInValueX).asDouble(),
			quatList.inputValue().child(attrInValueY).asDouble(),
			quatList.inputValue().child(attrInValueZ).asDouble(),
			quatList.inputValue().child(attrInValueW).asDouble());
		outputQuat *= quatVal;
	}

	
	MDataHandle attrHandle = data.outputValue(attrOutValue);

	attrHandle.child(attrOutValueX).setDouble(outputQuat.x);
	attrHandle.child(attrOutValueY).setDouble(outputQuat.y);
	attrHandle.child(attrOutValueZ).setDouble(outputQuat.z);
	attrHandle.child(attrOutValueW).setDouble(outputQuat.w);

	attrHandle.setClean();

	return MS::kSuccess;
}

MStatus QuaternionProduct::initialize() {
	MStatus stat;
	MFnNumericAttribute nAttr;
	MFnCompoundAttribute cmpAttr;
	//TODO: add attributeAffects to all child attributes
	attrOutValueX = nAttr.create("outValueX", "outValueX", MFnNumericData::kDouble);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutValueX);
	attrOutValueY = nAttr.create("outValueY", "outValueY", MFnNumericData::kDouble);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutValueY);
	attrOutValueZ = nAttr.create("outValueZ", "outValueZ", MFnNumericData::kDouble);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutValueZ);
	attrOutValueW = nAttr.create("outValueW", "outValueW", MFnNumericData::kDouble);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutValueW);

	attrOutValue = cmpAttr.create("outValue", "outValue");
	cmpAttr.addChild(attrOutValueX);
	cmpAttr.addChild(attrOutValueY);
	cmpAttr.addChild(attrOutValueZ);
	cmpAttr.addChild(attrOutValueW);
	cmpAttr.setWritable(false);
	cmpAttr.setStorable(false);
	addAttribute(attrOutValue);
	
	attrInValueX = nAttr.create("inValueX", "inValueX",  MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	addAttribute(attrInValueX);
	attributeAffects(attrInValueX, attrOutValue);

	attrInValueY = nAttr.create("inValueY", "inValueY", MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	addAttribute(attrInValueY);
	attributeAffects(attrInValueY, attrOutValue);

	attrInValueZ = nAttr.create("inValueZ", "inValueZ", MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	addAttribute(attrInValueZ);
	attributeAffects(attrInValueZ, attrOutValue);

	attrInValueW = nAttr.create("inValueW", "inValueW", MFnNumericData::kDouble, 1.0);
	nAttr.setKeyable(true);
	addAttribute(attrInValueW);
	attributeAffects(attrInValueW, attrOutValue);

	attrInValue = cmpAttr.create("inValue", "inValue");
	cmpAttr.addChild(attrInValueX);
	cmpAttr.addChild(attrInValueY);
	cmpAttr.addChild(attrInValueZ);
	cmpAttr.addChild(attrInValueW);
	cmpAttr.setArray(true);
	cmpAttr.setKeyable(true);
	addAttribute(attrInValue);
	cmpAttr.setUsesArrayDataBuilder(true);
	attributeAffects(attrInValue, attrOutValue);


	return MS::kSuccess;
}