
#include "aimConstraintNode.h"

#include <maya\MGlobal.h>
#include <math.h>

MTypeId AimConstraint::id(0x10d5);

MObject AimConstraint::attrInAimMatrix;
MObject AimConstraint::attrInOffsetMatrix;
MObject AimConstraint::attrInParentMatrix;
MObject AimConstraint::attrInUpMatrix;
//MObject AimConstraint::attrInWorldMatrix;


MObject AimConstraint::attrOutRotate;
MObject AimConstraint::attrOutRotateX;
MObject AimConstraint::attrOutRotateY;
MObject AimConstraint::attrOutRotateZ;

MObject AimConstraint::attrInTranslate;
MObject AimConstraint::attrInTranslateX;
MObject AimConstraint::attrInTranslateY;
MObject AimConstraint::attrInTranslateZ;

AimConstraint::AimConstraint() {
};

AimConstraint::~AimConstraint() {
}

void* AimConstraint::creator() {
	return new AimConstraint();
}

MStatus AimConstraint::compute(const MPlug& plug, MDataBlock& data) {
	MStatus stat;
	/*
	if (plug != attrOutRotate || plug != attrOutRotateX || plug != attrOutRotateY || plug != attrOutRotateZ)
	{
		return MS::kInvalidParameter;
	}
	*/
	MMatrix parentMatrix = data.inputValue(attrInParentMatrix).asMatrix();
	MMatrix offsetMatrix = data.inputValue(attrInOffsetMatrix).asMatrix();
	MMatrix aimMatrix = data.inputValue(attrInAimMatrix).asMatrix();
	MMatrix upMatrix = data.inputValue(attrInUpMatrix).asMatrix();

	MVector aimPos = MTransformationMatrix(aimMatrix).getTranslation(MSpace::kWorld);
	MVector upPos = MTransformationMatrix(upMatrix).getTranslation(MSpace::kWorld);
	
	float localMatrixVal[4][4] = { {1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{data.inputValue(attrInTranslateX).asFloat(), 
		data.inputValue(attrInTranslateY).asFloat(),
		data.inputValue(attrInTranslateZ).asFloat(), 1 }
	};
	MMatrix localMatrix(localMatrixVal);
	MMatrix worldMatrix = localMatrix * parentMatrix;

	//MVector srcPos = MVector(data.inputValue(attrInTranslateX).asFloat(),
	//						data.inputValue(attrInTranslateY).asFloat(),
	//						data.inputValue(attrInTranslateZ).asFloat());
	MVector srcPos = MTransformationMatrix(worldMatrix).getTranslation(MSpace::kWorld);

	/*
	char txt[512];
	//sprintf_s(txt, 512, "srcPos %f %f %f,", srcPos.x, srcPos.y, srcPos.z);
	sprintf_s(txt, 512, "localMtx %f %f %f,", localMatrix[4][0], localMatrix[4][1], localMatrix[4][2]);
	MGlobal::displayInfo(txt);
	*/

	MVector aimVec;
	MVector upVec;
	MVector upVecCross;
	MVector crossVec;
	
	aimVec = aimPos - srcPos;
	upVec = upPos - srcPos;
	crossVec = aimVec ^ upVec;
	upVecCross = crossVec ^ upVec;

	double outMatrixVal[4][4] = { {aimVec.x, aimVec.y, aimVec.z, 0},
			{upVec.x, upVec.y, upVec.z, 0},
			{crossVec.x, crossVec.y, crossVec.z, 0},
			{0, 0, 0, 1 }
	};
	MMatrix outMatrix(outMatrixVal);


	MEulerRotation outRotate = MTransformationMatrix(outMatrix * offsetMatrix).eulerRotation();

	MDataHandle hOutputX = data.outputValue(attrOutRotateX);
	MDataHandle hOutputY = data.outputValue(attrOutRotateY);
	MDataHandle hOutputZ = data.outputValue(attrOutRotateZ);
	


	//hOutputX.setFloat(outRotate.x * (180.0 / 3.141592653589793238463));
	hOutputX.set(outRotate.x);
	hOutputX.setClean();
	
	hOutputY.set(outRotate.y);
	hOutputY.setClean();
	
	hOutputZ.set(outRotate.z);
	hOutputZ.setClean();
	
	data.setClean(plug);

	return MS::kSuccess;
}

MStatus AimConstraint::initialize() {
	MStatus stat;
	MFnNumericAttribute nAttr;
	MFnCompoundAttribute cmpAttr;
	MFnMatrixAttribute mAttr;
	MFnUnitAttribute uAttr;
	
	// output rotates
	attrOutRotateX = uAttr.create("rotateX", "rotateX", MFnUnitAttribute::kAngle);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutRotateX);

	attrOutRotateY = uAttr.create("rotateY", "rotateY", MFnUnitAttribute::kAngle);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutRotateY);

	attrOutRotateZ = uAttr.create("rotateZ", "rotateZ", MFnUnitAttribute::kAngle);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutRotateZ);	

	attrOutRotate = cmpAttr.create("rotate", "rotate");
	cmpAttr.addChild(attrOutRotateX);
	cmpAttr.addChild(attrOutRotateY);
	cmpAttr.addChild(attrOutRotateZ);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(attrOutRotate);

	// input transalte
	attrInTranslateX = nAttr.create("translateX", "translateX", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrInTranslateX);
	attributeAffects(attrInTranslateX, attrOutRotateX);
	attributeAffects(attrInTranslateX, attrOutRotateY);
	attributeAffects(attrInTranslateX, attrOutRotateZ);
	attributeAffects(attrInTranslateX, attrOutRotate);

	attrInTranslateY = nAttr.create("translateY", "translateY", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrInTranslateY);
	attributeAffects(attrInTranslateY, attrOutRotateX);
	attributeAffects(attrInTranslateY, attrOutRotateY);
	attributeAffects(attrInTranslateY, attrOutRotateZ);
	attributeAffects(attrInTranslateY, attrOutRotate);

	attrInTranslateZ = nAttr.create("translateZ", "translateZ", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(attrInTranslateZ);
	attributeAffects(attrInTranslateZ, attrOutRotateX);
	attributeAffects(attrInTranslateZ, attrOutRotateY);
	attributeAffects(attrInTranslateZ, attrOutRotateZ);
	attributeAffects(attrInTranslateZ, attrOutRotate);

	attrInTranslate = cmpAttr.create("translate", "translate");
	cmpAttr.addChild(attrInTranslateX);
	cmpAttr.addChild(attrInTranslateY);
	cmpAttr.addChild(attrInTranslateZ);
	cmpAttr.setKeyable(true);
	addAttribute(attrInTranslate);
	attributeAffects(attrInTranslate, attrOutRotateX);
	attributeAffects(attrInTranslate, attrOutRotateY);
	attributeAffects(attrInTranslate, attrOutRotateZ);
	attributeAffects(attrInTranslate, attrOutRotate);

	/*
	attrInWorldMatrix = mAttr.create("worldMatrix", "worldMatrix");
	mAttr.setDefault(MMatrix::identity);
	addAttribute(attrInWorldMatrix);
	attributeAffects(attrInWorldMatrix, attrOutRotateX);
	attributeAffects(attrInWorldMatrix, attrOutRotateY);
	attributeAffects(attrInWorldMatrix, attrOutRotateZ);
	attributeAffects(attrInWorldMatrix, attrOutRotate);	
	*/

	attrInParentMatrix = mAttr.create("parentMatrix", "parentMatrix");
	mAttr.setDefault(MMatrix::identity);
	addAttribute(attrInParentMatrix);
	attributeAffects(attrInParentMatrix, attrOutRotateX);
	attributeAffects(attrInParentMatrix, attrOutRotateY);
	attributeAffects(attrInParentMatrix, attrOutRotateZ);
	attributeAffects(attrInParentMatrix, attrOutRotate);

	attrInOffsetMatrix = mAttr.create("offsetMatrix", "offsetMatrix");
	mAttr.setDefault(MMatrix::identity);
	addAttribute(attrInOffsetMatrix);
	attributeAffects(attrInOffsetMatrix, attrOutRotateX);
	attributeAffects(attrInOffsetMatrix, attrOutRotateY);
	attributeAffects(attrInOffsetMatrix, attrOutRotateZ);
	attributeAffects(attrInOffsetMatrix, attrOutRotate);

	attrInAimMatrix = mAttr.create("aimMatrix", "aimMatrix");
	mAttr.setDefault(MMatrix::identity);
	addAttribute(attrInAimMatrix);
	attributeAffects(attrInAimMatrix, attrOutRotateX);
	attributeAffects(attrInAimMatrix, attrOutRotateY);
	attributeAffects(attrInAimMatrix, attrOutRotateZ);
	attributeAffects(attrInAimMatrix, attrOutRotate);

	attrInUpMatrix = mAttr.create("upMatrix", "upMatrix");
	mAttr.setDefault(MMatrix::identity);
	addAttribute(attrInUpMatrix);
	attributeAffects(attrInUpMatrix, attrOutRotateX);
	attributeAffects(attrInUpMatrix, attrOutRotateY);
	attributeAffects(attrInUpMatrix, attrOutRotateZ);
	attributeAffects(attrInUpMatrix, attrOutRotate);

	return MS::kSuccess;

}