#pragma once

#ifndef GEMMSURFACEMESHREADER_H
#define GEMMSURFACEMESHREADER_H

#include <string>
#include "GemmMainFrame.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

// VTK forward class declarations
class vtkPolyDataMapper;
class vtkActor;

class GemmSurfaceMeshReader
{
public:
	enum class GemmSurfaceMeshReaderType {
		STL,
		PROSTAR3,
		PROSTAR4
	};
	GemmSurfaceMeshReader(GemmMainFrame* frame, const GemmSurfaceMeshReaderType &type);
	GemmMainFrame* getGemmMainFrame();
	GemmSurfaceMeshReaderType getGemmSurfaceMeshReaderType();
	void render(vtkSmartPointer<vtkRenderer> renderer);

protected:
	vtkPolyDataMapper* mapper;
	vtkActor* actor;

private:
	GemmMainFrame* gemmMainFrame;
	GemmSurfaceMeshReaderType gemmSurfaceMeshReaderType;
};
#endif

