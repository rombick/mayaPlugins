#ifndef QUATERNIONPRODUCT_H
#define QUATERNIONPRODUCT_H

#include <maya\MPxNode.h>
#include <maya\MFnNumericAttribute.h>
#include <maya\MFnCompoundAttribute.h>
#include <maya\MQuaternion.h>
#include <maya\MGlobal.h>
#include <maya/MArrayDataBuilder.h>


class QuaternionProduct : public MPxNode{
public:
	QuaternionProduct();

	virtual ~QuaternionProduct();
	static void* creator();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static MStatus initialize();

	static MTypeId id;
	static MObject attrOutValue;
	static MObject attrOutValueX;
	static MObject attrOutValueY;
	static MObject attrOutValueZ;
	static MObject attrOutValueW;
	static MObject attrInValue;
	static MObject attrInValueX;
	static MObject attrInValueY;
	static MObject attrInValueZ;
	static MObject attrInValueW;

};


#endif