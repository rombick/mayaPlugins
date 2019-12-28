/*
	filename:
		hello_maya.h
	description:
		This file contains the definition for a new command which the user of this plugin
		will be able to call from within Maya. The definitions for the class methods are
		contained in hello_maya.cpp.
*/

#ifndef HELLO_MAYA_H_
#define HELLO_MAYA_H_

#include <maya\MPxCommand.h>

// A very, very basic Maya command.
class HelloMaya : public MPxCommand {
public:
	HelloMaya() {};

	// This is the function that gets called when the Maya user calls 'HelloMaya' from MEL.
	virtual MStatus	doIt(const MArgList&);

	// This method is called when the command is registered.
	// It should return a pointer to a new instance of the class.
	static void* creator();
};


#endif