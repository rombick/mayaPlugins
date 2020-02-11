

#ifndef AIMCONSTRAINTNODE_H
#define AIMCONSTRAINTNODE_H

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnAttribute.h>

class AimConstraint : public MPxNode
{
public:
	AimConstraint();

	virtual ~AimConstraint();
	static void* creator();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static MStatus initialize();
	
	static MMatrix placeVectorsInMatrix(const short& aim, const short& up, const MVector& aimVec, const MVector& upVec, const MVector& prodVec);
	static MEulerRotation noFlip(const MVector& aimVec, const MVector& aimAxis, const MVector& upPos);

	static MTypeId id;
	static MObject attrOutRotate;
	static MObject attrOutRotateX;
	static MObject attrOutRotateY;
	static MObject attrOutRotateZ;

	static MObject attrInAimAxis;
	static MObject attrInUpAxis;
	static MObject attrUpType;

	static MObject attrInTranslate;
	static MObject attrInTranslateX;
	static MObject attrInTranslateY;
	static MObject attrInTranslateZ;

	static MObject attrInFlipUp;
	static MObject attrInFlipAim;

	//static MObject attrInWorldMatrix;
	static MObject attrInParentMatrix;
	static MObject attrInOffsetMatrix;
	static MObject attrInAimMatrix;
	static MObject attrInUpMatrix;
};

#endif