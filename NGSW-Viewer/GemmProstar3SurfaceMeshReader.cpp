#include <fstream>
#include <iostream>
#include <sstream>

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include "GemmProstar3SurfaceMeshReader.h"

//using namespace std;

GemmProstar3SurfaceMeshReader::GemmProstar3SurfaceMeshReader(GemmMainFrame* frame, const std::string& vrtFileName, const std::string& celFileName)
    : GemmSurfaceMeshReader(frame, GemmSurfaceMeshReader::GemmSurfaceMeshReaderType::PROSTAR3)
{
    readSTARVertices(vrtFileName, &vertexMap, &vertexPoints);
    nv = vertexPoints->GetNumberOfPoints();
    readSTARCells(celFileName, vertexMap, nv, &cellDefs);
    nc = cellDefs->GetNumberOfCells();
    activated = true;

    polydata = vtkPolyData::New();
    polydata->SetPoints(vertexPoints);
    polydata->SetPolys(cellDefs);

    mapper = vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);

    actor = vtkActor::New();
    actor->SetMapper(mapper);

    //getGemmMainFrame()->getRenderer()->AddActor(actor);
    //getGemmMainFrame()->getRenderer()->ResetCamera();
}


void GemmProstar3SurfaceMeshReader::setActivated(bool status)
{
    activated = status;
}


int GemmProstar3SurfaceMeshReader::getNumberOfVertices()
{
    return nv;
}


int GemmProstar3SurfaceMeshReader::getNumberOfCells()
{
    return nc;
}


int* GemmProstar3SurfaceMeshReader::getVertexMap()
{
    return vertexMap;
}


vtkPoints* GemmProstar3SurfaceMeshReader::getVertexPoints()
{
    return vertexPoints;
}


vtkCellArray* GemmProstar3SurfaceMeshReader::getCellDefs()
{
    return cellDefs;
}


vtkActor* GemmProstar3SurfaceMeshReader::getActor()
{
    return actor;
}


int GemmProstar3SurfaceMeshReader::linearSearch(int value, int* array, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        if (array[i] == value) return i;
    }
    return -1;
}


void GemmProstar3SurfaceMeshReader::readSTARVertices(const std::string& vrtFileName,
    int** vrtMap,
    vtkPoints** vrtPoints)
{
    int   iv, nv;
    double x, y, z, xyz[3];
    std::string line;
    ifstream vrtFile;

    vrtFile.open(vrtFileName);
    nv = 0;
    while (!vrtFile.eof())
    {
        getline(vrtFile, line);
        if (line.size() >= 63) nv++;
    }

    *vrtPoints = vtkPoints::New();
    (*vrtPoints)->SetNumberOfPoints(nv);
    *vrtMap = new int[nv];

    nv = 0;
    vrtFile.clear();
    vrtFile.seekg(0, ios::beg);
    while (!vrtFile.eof())
    {
        getline(vrtFile, line);
        if (line.size() >= 63)
        {
            nv++;
            std::istringstream istiv(line.substr(1, 9));
            istiv >> iv;
            (*vrtMap)[nv - 1] = iv;
            std::istringstream istx(line.substr(15, 16));
            istx >> x;
            std::istringstream isty(line.substr(31, 16));
            isty >> y;
            std::istringstream istz(line.substr(47, 16));
            istz >> z;
            (*vrtPoints)->SetPoint(nv - 1, x, y, z);
        }
    }
    vrtFile.close();

    return;
}


void GemmProstar3SurfaceMeshReader::readSTARCells(const std::string& celFileName,
    int* vrtMap, int NumVerts,
    vtkCellArray** celDefs)
{
    int   i, ic, ivc[4], ivu[4], nc, start, step;
    std::string line;
    ifstream celFile;

    celFile.open(celFileName);
    nc = 0;
    while (!celFile.eof())
    {
        getline(celFile, line);
        if (line.size() >= 101) nc++;
    }

    *celDefs = vtkCellArray::New();

    nc = 0;
    celFile.clear();
    celFile.seekg(0, ios::beg);
    while (!celFile.eof())
    {
        getline(celFile, line);
        if (line.size() >= 101)
        {
            nc++;
            (*celDefs)->InsertNextCell(4);
            std::istringstream istic(line.substr(0, 9));
            istic >> ic;
            start = 15;
            step = 9;
            for (i = 1; i <= 4; i++)
            {
                std::istringstream istiv(line.substr(start, step));
                istiv >> ivu[i - 1];
                ivc[i - 1] = linearSearch(ivu[i - 1], vrtMap, NumVerts);
                (*celDefs)->InsertCellPoint(ivc[i - 1]);
                start += step;
            }
        }
    }
    celFile.close();

    return;
}