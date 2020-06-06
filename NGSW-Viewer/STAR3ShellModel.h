#ifndef STAR3SHELLMODEL_H
#define STAR3SHELLMODEL_H

#include <cstring>

// VTK forward class declarations
class vtkPoints;
class vtkCellArray;

class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;

class STAR3ShellModel
{
public:
    STAR3ShellModel(const std::string& vrtFileName, const std::string& celFileName);
    int GetNumberOfVertices();
    int GetNumberOfCells();
    int* GetVertexMap();
    vtkPoints* GetVertexPoints();
    vtkCellArray* GetCellDefs();
    void SetActivated(bool status);
    vtkActor* GetActor();

private:
    int LinearSearch(int value, int* array, int array_size);
    void readSTARVertices(const std::string& vrtFileName, int** vrtMap, vtkPoints** vrtPoints);
    void readSTARCells(const std::string& celFileName, int* vrtMap, int NumVerts, vtkCellArray** celDefs);

    int nv;                     // Number of vertices
    int nc;                     // Number of cells
    int* VertexMap;
    vtkPoints* VertexPoints;
    vtkCellArray* CellDefs;
    bool activated;

    vtkPolyData* polydata;
    vtkPolyDataMapper* mapper;
    vtkActor* actor;
};

#endif