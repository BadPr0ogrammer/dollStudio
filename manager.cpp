#include "manager.h"
#include "vtkitem.h"

using namespace DS;

void Manager::openSource(const QUrl &url)
{
    _vtk->_fname = url.toLocalFile();
    _vtk->openSource();
}

void Manager::playFlag()
{
    _vtk->play();
}
