#include "GemmSurfaceMeshReader.h"

GemmSurfaceMeshReader::GemmSurfaceMeshReader(GemmMainFrame *frame, const GemmSurfaceMeshReaderType &type)
{
	gemmMainFrame = frame;
	gemmSurfaceMeshReaderType = type;
}

GemmMainFrame* GemmSurfaceMeshReader::getGemmMainFrame()
{
	return gemmMainFrame;
}

GemmSurfaceMeshReader::GemmSurfaceMeshReaderType GemmSurfaceMeshReader::getGemmSurfaceMeshReaderType()
{
	return gemmSurfaceMeshReaderType;
}

void GemmSurfaceMeshReader::render(vtkSmartPointer<vtkRenderer> renderer)
{
	renderer->AddActor(actor);
	renderer->ResetCamera();
}
