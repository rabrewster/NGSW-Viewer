#pragma once

#ifndef GEMMPROSTAR3SURFACEMESHREADER_H
#define GEMMPROSTAR3SURFACEMESHREADER_H

#include "GemmSurfaceMeshReader.h"
#include <cstring>

// VTK forward class declarations
class vtkPoints;
class vtkCellArray;

class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;

class GemmProstar3SurfaceMeshReader : public GemmSurfaceMeshReader
{
public:
    GemmProstar3SurfaceMeshReader(GemmMainFrame* frame, const std::string& vrtFileName, const std::string& celFileName);
    int getNumberOfVertices();
    int getNumberOfCells();
    int* getVertexMap();
    vtkPoints* getVertexPoints();
    vtkCellArray* getCellDefs();
    void setActivated(bool status);
    vtkActor* getActor();

private:
    int linearSearch(int value, int* array, int array_size);
    void readSTARVertices(const std::string& vrtFileName, int** vrtMap, vtkPoints** vrtPoints);
    void readSTARCells(const std::string& celFileName, int* vrtMap, int NumVerts, vtkCellArray** celDefs);

    int nv;                     // Number of vertices
    int nc;                     // Number of cells
    int* vertexMap;
    vtkPoints* vertexPoints;
    vtkCellArray* cellDefs;
    bool activated;

    vtkPolyData* polydata;
    //vtkPolyDataMapper* mapper;
    //vtkActor* actor;
};

#endif