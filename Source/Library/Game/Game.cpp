#include "Game/Game.h"

namespace library
{
    /*--------------------------------------------------------------------
      Function definitions
    --------------------------------------------------------------------*/

    /*
    void PrintHi()
    {
        OutputDebugString(L"hi\n");
        MessageBox(nullptr, L"hi", L"Game Graphics Programming", MB_OK);
    }
    
    */

    D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;

    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11Device1* g_pd3dDevice1 = nullptr;
    ID3D11DeviceContext* g_pImmediateContext = nullptr;
    ID3D11DeviceContext1* g_pImmediateContext1 = nullptr;

    IDXGISwapChain* g_pSwapChain = nullptr;
    IDXGISwapChain1* g_pSwapChain1 = nullptr;

    ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

    LPCWSTR g_pszWindowClassName = L"GGPWindowClass";

    HWND g_hWnd = nullptr;
    HINSTANCE g_hInstance = nullptr;
    LPCWSTR g_pszWindowName = L"GGP02: Direct3D 11 Basics";


    HRESULT InitWindow(HINSTANCE hInstance, INT nCmdShow)
    {
        WNDCLASSEX wcex =
        {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = WindowProc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = hInstance,
            .hIcon = LoadIcon(hInstance, IDI_APPLICATION),
            .hCursor = LoadCursor(nullptr, IDC_ARROW),
            .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW +1),
            .IpszMenuName = nullptr,
            .IpszClassName = g_pszWindowClassName,
            .hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION),

        };

        if (!RegisterClassEx(&wcex))
        {
            DWORD dwError = GetLastError();
            MessageBox(
                nullptr,
                L"Call to RegisterClassEx failed!",
                L"Game Graphics Programming",
                NULL
            );
            if (dwError != ERROR_CLASS_ALREADY_EXISTS)
            {
                return HRESULT_FROM_WIN32(dwError);
            }
            return E_FAIL;
        }

        g_hInstance = hInstance;
        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        g_hWnd = CreateWindow(
            g_pszWindowClassName,
            g_pszWindowName,
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

        if (!g_hWnd)
        {
            DWORD dwError = GetLastError();
            MessageBox(
                nullptr,
                L"Call to CreateWindow failed!",
                L"Game Graphics Programming",
                NULL
            );
            if (dwError != ERROR_CLASS_ALREADY_EXISTS)
            {
                return HRESULT_FROM_WIN32(dwError);
            }
            return E_FAIL;

        }

        ShowWindow(g_hWnd, nCmdShow);
        return S_OK;

    }
    

    HRESULT InitDevice()
    {
        // Create D3D 11 device & context
        // Obtain DXGI Factory from device
        // Create swap chain
        // Create render target view
        // Setup the viewport

        HRESULT hr = S_OK;

        RECT rc;
        GetClientRect(g_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            g_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels,
                numFeatureLevels, D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
            if (hr == E_INVALIDARG)
            {
                hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1],
                    numFeatureLevels - 1, D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
            }
            if (SUCCEEDED(hr))
                break;
        }
      
        if (FAILED(hr))
            return hr;










        return E_NOTIMPL;
    }

    void CleanupDevice()
    {

    }
    void Render()
    {

    }
}