#include <windows.h>

int WinMain(HINSTANCE hInstance, 
                    HINSTANCE hPrevInstance, 
                    LPSTR pCmdLine, 
                    int nCmdShow
)
{
    MessageBox(0, "This is the first message from our game to us with love.", "2D game", MB_OK | MB_ICONINFORMATION);
    return(0);
}