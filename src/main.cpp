#include "gizmo.hpp"
#include "mesh.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    int runForSeconds = 25;

    if (argc > 1)
    {
        try
        {
            runForSeconds = std::stoi(argv[1]);
        }
        catch (const std::invalid_argument& e)
        {
            std::string title = "Invalid argument. Using default value of " + std::to_string(runForSeconds) + " seconds.";
            std::cout << "\033[35m" << title << "\033[0m" << std::endl;
        }
    }

    // utils::format::clearConsole();
    utils::format::printBegin();
    if (runForSeconds > 0) utils::timer::shutdown(runForSeconds);
    
    const std::string filePath = utils::disk::getCurrentDirectory() + "/models/cone.glb";

    Gizmo gizmo;
    if (gizmo.init() != 0)
    {
        std::cerr << "Failed to initialize Gizmo" << std::endl;
        return -1;
    }

    // gizmo.loadModel(filePath);
    // gizmo.loadMesh(filePath);
    utils::printGLVersion();
    if (gizmo.loadModel(filePath)) gizmo.run();
    
    utils::format::printEnd();

    return 0;
}