/*
	filename:
		plugin_main.cpp
	description:
		Contains two functions, initializePlugin and uninitializePlugin, which are called
		by Maya when it loads and unloads the plugin. As such, this file can be thought of
		as a 'front-end' for the plugin, similar to the way a 'main' file is the entry point
		to a program.

		Each time a new custom command is created and added to the plugin it needs to be
		registered and deregistered by these functions. The same goes for custom nodes.
*/

#include <maya\MGlobal.h>
#include <maya\MFnPlugin.h>

#include "hello_maya.h"

// Registers all the plugin's commands, nodes etc with the Maya API.
MStatus initializePlugin(MObject obj) {
	MFnPlugin plugin(obj, "Roman Rajbhandari", "Any");
	MStatus	stat;

	// Register HelloMaya command with the Maya API.
	stat = plugin.registerCommand("HelloMaya", HelloMaya::creator);
	// The user-defined HelloMaya command, defined as the function HelloMaya::doIt in 
	// hello_maya.cpp can now be called from within Maya by calling it from a MEL script.

	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return stat;
}

// De-registers all the plugin's commands, nodes etc from the Maya API.
MStatus uninitializePlugin(MObject obj) {
	MFnPlugin plugin(obj);
	MStatus	stat;

	// Remove HelloMaya command from the Maya API environment.
	stat = plugin.deregisterCommand("HelloMaya");
	// It is no longer possible to call the HelloMaya command in Maya.

	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return stat;
}