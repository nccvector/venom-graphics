#include <pybind11/pybind11.h>

#include <pybind11/numpy.h>

#include "OgreApp.h"

namespace py = pybind11;

class PythonAppHandler
{
  public:
    PythonAppHandler()
    {
        app = new MyTestApp();
        app->initApp();

        // Rendering one frame just for some settings to take effect (windowHidden->true)
        app->getNextFrame();
    }

    ~PythonAppHandler()
    {
        app->closeApp();
        delete app;
    }

    py::array_t<uint8_t> getRenderTexture()
    {
        void *data = const_cast<void *>(app->getNextFrame());
        auto result = py::array(py::buffer_info(data, /* Pointer to data (nullptr -> ask NumPy to allocate!) */
                                                sizeof(uint8_t),                       /* Size of one item */
                                                py::format_descriptor<uint8_t>::value, /* Buffer format */
                                                3,                                     /* How many dimensions? */
                                                {640, 480, 3}, /* Number of elements for each dimension */
                                                {sizeof(unsigned char) * 3, sizeof(unsigned char) * 640 * 3,
                                                 sizeof(unsigned char)} /* Strides for each dimension */
                                                ));
        return result;
    }

  private:
    MyTestApp *app;
};

PYBIND11_MODULE(pymugen, m)
{
    py::class_<PythonAppHandler>(m, "PythonAppHandler", py::buffer_protocol())
        .def(py::init<>())
        .def("getNextFrame", &PythonAppHandler::getRenderTexture, "Returns next frame");
}