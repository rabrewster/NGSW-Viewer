#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include "GemmVisPanel.h"
#include "STAR3ShellModel.h"

#define MY_PANEL      101
#define MY_VTK_WINDOW 102

#define ID_GemmVisPanel_TIMER 1001

// constructors
GemmVisPanel::GemmVisPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, wxID_ANY, pos, size)
{
    wxSize initialSize = wxSize(720, 480);
    renderWindowInteractor = new wxVTKRenderWindowInteractor(this, MY_VTK_WINDOW, wxPoint(0, 0), size);

    ConstructVTK();
    ConfigureVTK();
    this->SetBackgroundColour(123);
}

GemmVisPanel::~GemmVisPanel()
{
    if (renderWindowInteractor) renderWindowInteractor->Delete();
    DestroyVTK();
}

void GemmVisPanel::ConstructVTK()
{
    renderer = vtkRenderer::New();
}

void GemmVisPanel::ConfigureVTK()
{
    // connect the render window and wxVTK window
    renderWindow = renderWindowInteractor->GetRenderWindow();
    // connect renderer and render window and configure render window
    renderWindow->SetSize(1000, 1000);
    renderWindow->AddRenderer(renderer);
    // read in geometry and set renderer
    newVessel();
    // configure renderer
    renderer->GradientBackgroundOff();
    renderer->SetBackground(0.75, 0.75, 0.75);
    renderer->SetBackground(0, 0, 0.75);
    renderer->GetActiveCamera()->Elevation(30.0);
    renderer->GetActiveCamera()->Azimuth(30.0);
    renderer->GetActiveCamera()->Zoom(1.0);
    renderer->GetActiveCamera()->SetClippingRange(1, 1000);
    renderer->ResetCamera();

}

void GemmVisPanel::DestroyVTK()
{
    // See: http://www.vtk.org/pipermail/vtkusers/2003-September/019894.html
    if (renderer != 0)
        renderer->Delete();
}

vtkSmartPointer<vtkRenderer> GemmVisPanel::GetRenderer()
{
    return renderer;
}

vtkSmartPointer<vtkRenderWindow> GemmVisPanel::GetRenderWindow()
{
    return renderWindow;
}

wxVTKRenderWindowInteractor* GemmVisPanel::GetRenderWindowInteractor()
{
    return renderWindowInteractor;
}

void GemmVisPanel::newFile()
{
}

void GemmVisPanel::newVessel()
{
    std::string vrtFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.vrt";
    std::string celFileName = "C:/Users/racbr/Documents/Northrop Grumman/NG-SHIPWAVE/Models/lancs.cel";

    if ((!vrtFileName.empty()) && (!celFileName.empty()))
    {
        model = new STAR3ShellModel(vrtFileName, celFileName);

        renderer->AddActor(model->GetActor());
        //        pRenderer->ResetCamera();
        //        pRenderer->GetRenderWindow()->Render();
        //        statusBar()->message(tr("Vessel file reads completed"), 2000);
    }
    else
    {
        //        statusBar()->message(tr("Vessel file reads canceled"), 2000);
    }
}


void GemmVisPanel::eraseVessel()
{
}

void GemmVisPanel::activateVessel()
{
    model->SetActivated(true);
    renderer->AddActor(model->GetActor());
    renderer->GetRenderWindow()->Render();
}


void GemmVisPanel::deactivateVessel()
{
    model->SetActivated(false);
    renderer->RemoveActor(model->GetActor());
    renderer->GetRenderWindow()->Render();
}
