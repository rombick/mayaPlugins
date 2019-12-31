

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

class AimConstraint : public MPxNode
{
public:
	AimConstraint();

	virtual ~AimConstraint();
	static void* creator();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static MStatus initialize();

	static MTypeId id;
	static MObject attrOutRotate;
	static MObject attrOutRotateX;
	static MObject attrOutRotateY;
	static MObject attrOutRotateZ;

	static MObject attrInTranslate;
	static MObject attrInTranslateX;
	static MObject attrInTranslateY;
	static MObject attrInTranslateZ;

	//static MObject attrInWorldMatrix;
	static MObject attrInParentMatrix;
	static MObject attrInOffsetMatrix;
	static MObject attrInAimMatrix;
	static MObject attrInUpMatrix;
};

#endif