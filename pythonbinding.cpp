#include <pybind11/pybind11.h>

#include <pybind11/numpy.h>

#include "OgreApp.h"

namespace py = pybind11;

class PythonAppHandler
{
  public:
    PythonAppHandler()
    {
    }

    ~PythonAppHandler()
    {
        app->closeApp();
        delete app;
    }

    void init(int windowHandle)
    {
        app = new MyTestApp(windowHandle);
        app->initApp();
        app->getRoot()->renderOneFrame();
    }

    void renderOneFrame()
    {
        app->getRoot()->renderOneFrame();
    }

    void resize(int x, int y)
    {
        app->resize(x, y);
    }

  private:
    MyTestApp *app;
};

PYBIND11_MODULE(PyVenom, m)
{
    py::class_<PythonAppHandler>(m, "PythonAppHandler", py::buffer_protocol())
        .def(py::init<>())
        .def("init", &PythonAppHandler::init, "Initializes Ogre3D")
        .def("renderOneFrame", &PythonAppHandler::renderOneFrame, "Renders one frame")
        .def("resize", &PythonAppHandler::resize, "Resizes RenderWindow");
}