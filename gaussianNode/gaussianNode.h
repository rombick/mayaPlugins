/*
	filename:
		hello_maya.h
	description:
		This file contains the definition for a new command which the user of this plugin
		will be able to call from within Maya. The definitions for the class methods are
		contained in hello_maya.cpp.
*/

#ifndef GAUSSIANNODE_H
#define GAUSSIANNODE_H

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>

class GaussianNode : public MPxNode
{
public:
	GaussianNode();

	virtual ~GaussianNode();
	static void* creator();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static MStatus initialize();
	
	static MTypeId id;
	static MObject attrOutValue;
	static MObject attrInValue;
	static MObject attrMagnitute;
	static MObject attrMean;
	static MObject attrVariance;
};

#endif