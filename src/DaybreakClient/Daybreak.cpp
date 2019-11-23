// Daybreak entry and main loop.
#include "stdafx.h"
#include "Daybreak.h"
#include "DaybreakApp.h"

#include <SDL.h>

//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    DaybreakApp app(800, 600);

#ifdef _DEBUG
    app.setGlDebugContextRequested(true);
#else
    app.setGlDebugContextRequested(false);
#endif

    // TODO: Use the logging system to print the working directory.
    // TODO: Support linux and other platforms for working directory.
    char currentPath[FILENAME_MAX];

    /*auto bytes =*/ GetModuleFileNameA(NULL, &currentPath[0], FILENAME_MAX);
    

    //try
    //{
         app.Run();
    //}
    /* TODO: Only catch if debugger attached
    // TODO: Catch file access errors, make sure to print the current working directory in the error message.
    catch (DaybreakEngineException& e)
    {
        std::string message;

        auto details = e.Details();

        if (!details.empty())
        {
            message = e.Message() + "\nDetails: " + details;
        }

        DisplayErrorDialog("Daybreak error", message);
    }
    catch (SdlException& e)
    {
        DisplayErrorDialog("SDL error", e.what());
    }
    catch (GladException& e)
    {
        DisplayErrorDialog("GLAD error", e.what());
    }
    catch (GlException& e)
    {
        DisplayErrorDialog("OpenGL error", e.what());
    }
    catch (std::runtime_error& e)
    {
        DisplayErrorDialog("Uncaught runtime exception", e.what());
    }*/

    return 0;
}
