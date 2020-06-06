#include <fstream>
#include <iostream>
#include <sstream>

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include "STAR3ShellModel.h"

//using namespace std;

STAR3ShellModel::STAR3ShellModel(const std::string& vrtFileName, const std::string& celFileName)
{
    readSTARVertices(vrtFileName, &VertexMap, &VertexPoints);
    nv = VertexPoints->GetNumberOfPoints();
    readSTARCells(celFileName, VertexMap, nv, &CellDefs);
    nc = CellDefs->GetNumberOfCells();
    activated = true;

    polydata = vtkPolyData::New();
    polydata->SetPoints(VertexPoints);
    polydata->SetPolys(CellDefs);

    mapper = vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);

    actor = vtkActor::New();
    actor->SetMapper(mapper);
}


void STAR3ShellModel::SetActivated(bool status)
{
    activated = status;
}


int STAR3ShellModel::GetNumberOfVertices()
{
    return nv;
}


int STAR3ShellModel::GetNumberOfCells()
{
    return nc;
}


int* STAR3ShellModel::GetVertexMap()
{
    return VertexMap;
}


vtkPoints* STAR3ShellModel::GetVertexPoints()
{
    return VertexPoints;
}


vtkCellArray* STAR3ShellModel::GetCellDefs()
{
    return CellDefs;
}


vtkActor* STAR3ShellModel::GetActor()
{
    return actor;
}


int STAR3ShellModel::LinearSearch(int value, int* array, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        if (array[i] == value) return i;
    }
    return -1;
}


void STAR3ShellModel::readSTARVertices(const std::string& vrtFileName,
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


void STAR3ShellModel::readSTARCells(const std::string& celFileName,
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
                ivc[i - 1] = LinearSearch(ivu[i - 1], vrtMap, NumVerts);
                (*celDefs)->InsertCellPoint(ivc[i - 1]);
                start += step;
            }
        }
    }
    celFile.close();

    return;
}