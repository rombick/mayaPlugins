 /*
	filename:
		hello_maya.cpp
	description:
		Contains definitions for the HelloMaya class methods declared in hello_maya.h.
*/

#include "hello_maya.h"

#include <maya\MGlobal.h>

// This function is called when the plugin is registered with the Maya API.
// All it does is return a pointer to a new instance of the class.
void* HelloMaya::creator() {
	return new HelloMaya;
}

// This function is called when the Maya user types 'HelloMaya' in MEL.
MStatus HelloMaya::doIt(const MArgList& args) {
	// Print a message to the Maya console.
	MGlobal::displayInfo("Hello Maya!");

	return MS::kSuccess;
}