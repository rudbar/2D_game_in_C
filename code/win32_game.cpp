#include <windows.h>

LRESULT CALLBACK 
MainWindowCallback(HWND   Window,
                   UINT   Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            static DWORD Operation = WHITENESS;
            PatBlt(DeviceContext, X, Y, Width, Height, Operation);
            if(Operation == WHITENESS)
            {
                Operation = BLACKNESS;
            }
            else
            {
                Operation = WHITENESS;
            }
            EndPaint(Window, &Paint);
        } break;

        default:
        {
//          OutputDebugStringA("default\n");
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

    return (Result);
}

int WinMain(HINSTANCE Instance, 
                    HINSTANCE PrevInstance, 
                    LPSTR CommandLine, 
                    int ShowCode)
{   
    // Creating the window
        WNDCLASS WindowClass = {}; // setting all the below variables of WNDCLASS to 0
    
        WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW; // style is a set of binary flags of 
                                                            // properties that we want our window to have
                                                            // CS_OWNDC Allocates a unique device context for each window in the class.
                                                            // CS_HREDRAW and CS_VREDRAW redraw the window if a movement occured
        WindowClass.lpfnWndProc = MainWindowCallback; // lpfnWndProc is a pointer to a function that we 
                                    // will define that defines how window responds to events
        WindowClass.hInstance = Instance; // hInstance A handle to the instance that contains the window procedure for the class.
    //  WindowClass.hIcon; - we don't need to set the icon now
        WindowClass.lpszClassName = "GameWindowClass"; // class name

        if(RegisterClass(&WindowClass))
        {
            HWND WindowHandle = 
                CreateWindowEx(
                    0,
                    WindowClass.lpszClassName,
                    "Tom the Man",
                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    0,
                    0,
                    Instance,
                    0);
            if(WindowHandle)
            {
                for(;;)
                {
                    MSG Message;
                    BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
                    if(MessageResult > 0)
                    {
                        TranslateMessage(&Message);
                        DispatchMessage(&Message);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
            }
        }
        else
        {

        }

    return(0);
}