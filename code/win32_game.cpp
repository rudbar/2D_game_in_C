#include <windows.h>

#define local_persist static
#define global_variable static
#define internal static

// Win32 prefix for functions meaning I defined those functions
// and they're not Windows reserved names
// Global variables
global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext; 

internal void Win32ResizeDIBSection(int Width, int Height)
{
    if(BitmapHandle)
    {
        DeleteObject(BitmapHandle);

    }
    
    if(!BitmapDeviceContext)
    {
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32; // in fact we need 24 bits, 8 bits per each color (8 bits for RED, 8 bits for GREEN, 8 bits for BLUE)
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext, &BitmapInfo,
        DIB_RGB_COLORS,
        &BitmapMemory,
        0, 0);
}

internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    StretchDIBits(DeviceContext, 
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  BitmapMemory,
                  &BitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK  Win32MainWindowCallback(HWND   Window,
                                          UINT   Message,
                                          WPARAM WParam,
                                          LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;
            Win32ResizeDIBSection(Width, Height);
        } break;

        case WM_CLOSE:
        {
            Running = false;
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_DESTROY:
        {
            Running = false;
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
            local_persist DWORD Operation = WHITENESS;
            PatBlt(DeviceContext, X, Y, Width, Height, Operation);
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
        WindowClass.lpfnWndProc = Win32MainWindowCallback; // lpfnWndProc is a pointer to a function that we 
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
                Running = true;
                while(Running)
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