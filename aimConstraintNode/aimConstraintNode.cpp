
#include "aimConstraintNode.h"

#include <maya\MGlobal.h>
#include <math.h>

MTypeId AimConstraint::id(0x10d5);

MObject AimConstraint::attrInAimMatrix;
MObject AimConstraint::attrInOffsetMatrix;
MObject AimConstraint::attrInParentMatrix;
MObject AimConstraint::attrInUpMatrix;
//MObject AimConstraint::attrInWorldMatrix;
MObject AimConstraint::attrInAimAxis;
MObject AimConstraint::attrInUpAxis;
MObject AimConstraint::attrUpType;

MObject AimConstraint::attrInFlipUp;
MObject AimConstraint::attrInFlipAim;

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

	bool flipAim = data.inputValue(attrInFlipAim).asBool();
	bool flipUp = data.inputValue(attrInFlipUp).asBool();

	short aimAxis = data.inputValue(attrInAimAxis).asShort();
	short upAxis = data.inputValue(attrInUpAxis).asShort();
	short upType = data.inputValue(attrUpType).asShort();


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



	MVector aimVec;
	MVector upVec;
	MVector upVecCross;
	MVector crossVec;
	MVector slaveVec;

	
	aimVec = aimPos - srcPos;
	if (flipAim == true) {
		aimVec = aimVec * -1;
	};

	upVec = upPos - srcPos;
	aimVec.normalize();
	upVec.normalize();

	crossVec = aimVec ^ upVec;
	upVecCross = crossVec ^ aimVec;
	slaveVec =  upVecCross ^ aimVec;


	if (flipUp == true) {
		upVecCross = upVecCross * -1;
	};

	MMatrix outMatrix = placeVectorsInMatrix(aimAxis, upAxis, aimVec, upVecCross, slaveVec);

	
	//char txt[512];
	//sprintf_s(txt, 512, "crossVec %f %f %f,", crossVec.x, crossVec.y, crossVec.z);
	//MGlobal::displayInfo(txt);

	double one = 1.0;
	double scaleVar[3] = {one, one, one};
	MTransformationMatrix OutTrMtx(outMatrix * offsetMatrix);

	double scaleOutVar[3];
	OutTrMtx.setScale(scaleOutVar, MSpace::kWorld);
	
	MEulerRotation outRotate = OutTrMtx.eulerRotation();
	if (upType == 2) {
		MVector testVec(1, 0, 0);
		outRotate = noFlip(aimVec, testVec, upPos);
	};


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
	MFnEnumAttribute enumAttr;
	MFnAttribute boolAttr;
	
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
	// enum attrs
	attrInAimAxis = enumAttr.create("aimAxis", "aimAxis");
	enumAttr.addField("x", 0);
	enumAttr.addField("y", 1);
	enumAttr.addField("z", 2);
	enumAttr.setKeyable(true);
	enumAttr.setStorable(true);
	addAttribute(attrInAimAxis);
	attributeAffects(attrInAimAxis, attrOutRotateX);
	attributeAffects(attrInAimAxis, attrOutRotateY);
	attributeAffects(attrInAimAxis, attrOutRotateZ);
	attributeAffects(attrInAimAxis, attrOutRotate);

	attrInUpAxis = enumAttr.create("upAxis", "upAxis");
	enumAttr.addField("x", 0);
	enumAttr.addField("y", 1);
	enumAttr.addField("z", 2);
	enumAttr.setKeyable(true);
	enumAttr.setStorable(true);
	addAttribute(attrInUpAxis);
	attributeAffects(attrInUpAxis, attrOutRotateX);
	attributeAffects(attrInUpAxis, attrOutRotateY);
	attributeAffects(attrInUpAxis, attrOutRotateZ);
	attributeAffects(attrInUpAxis, attrOutRotate);

	attrUpType = enumAttr.create("upType", "upType");
	enumAttr.addField("upObject", 0);
	enumAttr.addField("upRotation", 1);
	enumAttr.addField("noUp", 2);
	enumAttr.setKeyable(true);
	enumAttr.setStorable(true);
	addAttribute(attrUpType);
	attributeAffects(attrUpType, attrOutRotateX);
	attributeAffects(attrUpType, attrOutRotateY);
	attributeAffects(attrUpType, attrOutRotateZ);
	attributeAffects(attrUpType, attrOutRotate);

	// bool attrs
	attrInFlipUp = nAttr.create("flipUp", "flipUp", MFnNumericData::kBoolean, false);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	addAttribute(attrInFlipUp);
	attributeAffects(attrInFlipUp, attrOutRotateX);
	attributeAffects(attrInFlipUp, attrOutRotateY);
	attributeAffects(attrInFlipUp, attrOutRotateZ);
	attributeAffects(attrInFlipUp, attrOutRotate);

	attrInFlipAim = nAttr.create("flipAim", "flipAim", MFnNumericData::kBoolean, false);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	addAttribute(attrInFlipAim);
	attributeAffects(attrInFlipAim, attrOutRotateX);
	attributeAffects(attrInFlipAim, attrOutRotateY);
	attributeAffects(attrInFlipAim, attrOutRotateZ);
	attributeAffects(attrInFlipAim, attrOutRotate);

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


MMatrix AimConstraint::placeVectorsInMatrix(const short& aim, const short& up, const MVector& aimVec, const MVector& upVec, const MVector& prodVec )
{	
	MVector orderedVects[3] = { prodVec , prodVec , prodVec };

	orderedVects[aim] = aimVec;
	orderedVects[up] = upVec;

	double outMatrixVal[4][4] = { {orderedVects[0].x, orderedVects[0].y, orderedVects[0].z, 0},
		{orderedVects[1].x, orderedVects[1].y, orderedVects[1].z, 0},
		{orderedVects[2].x, orderedVects[2].y, orderedVects[2].z, 0},
		{0, 0, 0, 1 }
	};
	MMatrix outMatrix(outMatrixVal);

	return outMatrix;
}

MEulerRotation AimConstraint::noFlip(const MVector& aimVec, const MVector& aimAxis, const MVector& upPos)
{
	MVector rotAxis = aimAxis ^ aimVec;
	rotAxis.normalize();

	//if (rotAxis.length() == 0) {
	//	return;
	//}
	float dot = aimAxis * aimVec;
	float ang = std::acosf(dot);
	
	// axis angle to quat
	//auto s = std::sinf(ang / 2);
	//MQuaternion outQuat(rotAxis.x, rotAxis.y, rotAxis.z, std::cosf(ang/2));
	MQuaternion outQuat;
	outQuat.setAxisAngle(rotAxis, ang);
	return outQuat.asEulerRotation();
}

