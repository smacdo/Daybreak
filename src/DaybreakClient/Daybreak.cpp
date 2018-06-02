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


    //try
    //{
        app.Run();
    //}
    /* TODO: Only catch if debugger attached
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
