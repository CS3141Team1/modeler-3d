#include <iostream>
#include <cmath>

#include "Boost.h"
#include "Application.h"
#include "IBackend.h"
#include "Modeler3D.h"
#include "Types.h"
#include "FileIO.h"

#include "Math/ModelerMath.h"
#include "SDL2/Sdl2Window.h"
#include "SDL2/Sdl2Backend.h"

using namespace Core;
using namespace std;

int main(int argc, char** asrgv)
{
    cout << "Starting Modeler3D" << endl;

    IBackend* backend = new Sdl2Backend();

    Modeler3D app(backend);
    app.Start();

    delete backend;

    cout << "Exiting Modeler3D" << endl;

    //ListDir("C:/");

    boost::filesystem::path obj("Assets/cube.obj");

    FileIO objIn;
    objIn.LoadObj(obj);
    objIn.SaveObj("CubeTest");


    return 0;
}
