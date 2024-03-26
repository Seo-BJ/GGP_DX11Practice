#include "Game/Game.h"

namespace library
{
    /*--------------------------------------------------------------------
      Function definitions
    --------------------------------------------------------------------*/

    void PrintHi()
    {
        OutputDebugString(L"hi\n");
        MessageBox(nullptr, L"hi", L"Game Graphics Programming", MB_OK);
    }
    
    
    
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
    LPCWSTR g_pszWindowName = L"2019103556 수학과 서범준";

    // 사전 세팅 (3)
    ID3D11VertexShader* g_pVertexShader = nullptr;
    ID3D11PixelShader* g_pPixelShader = nullptr;
    ID3D11Buffer* g_pVertexBuffer = nullptr;
    ID3D11InputLayout* g_pVertexLayout = nullptr;
    ID3DBlob* pPixelShaderBlob = nullptr;
    ID3DBlob* pVertexShaderBlob = nullptr;





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
            .lpszMenuName = nullptr,
            .lpszClassName = g_pszWindowClassName,
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
      
      

        IDXGIFactory1* dxgiFactory = nullptr;
        IDXGIDevice* dxgiDevice = nullptr;
        hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
            if (SUCCEEDED(hr))
            {
                IDXGIAdapter* adapter = nullptr;
                hr = dxgiDevice->GetAdapter(&adapter);
                if (SUCCEEDED (hr))
                {
                    hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                    adapter->Release();
                }
                dxgiDevice->Release();
            }
        if (FAILED(hr))
            return hr;
        IDXGIFactory2* dxgiFactory2 = nullptr;
        hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
        if (dxgiFactory2)
        {     
            // DirectX 11.1 or later
            hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&g_pd3dDevice1));
            if (SUCCEEDED(hr))
            {
                (void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1),
                    reinterpret_cast<void**>(&g_pImmediateContext1));
            }
            DXGI_SWAP_CHAIN_DESC1 sd = {};
            sd.Width = width;
            sd.Height = height;
            sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount = 1;
            hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
            if (SUCCEEDED (hr))     
            {
                hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain),
                    reinterpret_cast<void**>(&g_pSwapChain));
            }
            dxgiFactory2->Release();
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC sd = {};
            sd.BufferCount = 1;
            sd.BufferDesc.Width = width;
            sd.BufferDesc.Height = height;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = g_hWnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;
            hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
        }
        dxgiFactory->Release();

        if (FAILED(hr))
            return hr;

        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (FAILED(hr))
            return hr;
        hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
        pBackBuffer->Release();
        if (FAILED(hr))
            return hr;
        g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
                        
        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)width;
        vp.Height = (FLOAT)height;
        vp.MinDepth = 0.f;
        vp.MaxDepth = 1.f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        g_pImmediateContext->RSSetViewports(1, &vp);

        hr = CompileShaderFromFile(L"../Library/Lab02.fx", "VS", "vs_5_0", &pVertexShaderBlob);
        hr = g_pd3dDevice->CreateVertexShader(
            pVertexShaderBlob->GetBufferPointer(),
            pVertexShaderBlob->GetBufferSize(),
            nullptr,
            &g_pVertexShader);
        if (FAILED(hr))
        {
            pVertexShaderBlob->Release();
            return hr;
        }

    
        ID3DBlob* pVertexShaderBlob = nullptr;
        D3D11_INPUT_ELEMENT_DESC layouts[] =
        {
            {"POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0},
        };

        UINT uNumElements = ARRAYSIZE(layouts);

        hr = g_pd3dDevice->CreateInputLayout(
            layouts,
            uNumElements,
            pVertexShaderBlob->GetBufferPointer(),
            pVertexShaderBlob->GetBufferSize(),
            &g_pVertexLayout
        );

        pVertexShaderBlob->Release();
        if (FAILED(hr))
            return hr;

        g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
       
   

        hr = CompileShaderFromFile(L"../Library/Lab02.fx", "PS", "ps_5_0", &pPixelShaderBlob);
        hr = g_pd3dDevice->CreatePixelShader(
            pPixelShaderBlob->GetBufferPointer(),
            pPixelShaderBlob->GetBufferSize(),
            nullptr,
            &g_pPixelShader);
        if (FAILED(hr))
        {
            pPixelShaderBlob->Release();
            return hr;
        }
        // 7
        SimpleVertex sVertices[] =
        {
            {XMFLOAT3(0.0f, 0.5f, 0.5f)},
            {XMFLOAT3(0.0f, -0.5f, 0.5f)},
            {XMFLOAT3(-0.0f, -0.5f, 0.5f)},
        };

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = sVertices;

        hr = g_pd3dDevice->CreateBuffer(
            &bd,
            &initData,
            &g_pVertexBuffer
        );
        if (FAILED(hr))
            return hr;
        // 8
        UINT uStride = sizeof(SimpleVertex);
        UINT uOffset = 0;
        g_pImmediateContext->IASetVertexBuffers(
            0,
            1,
            &g_pVertexBuffer,
            &uStride,
            &uOffset
        );
        // 9 
        g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        return  S_OK;               
    }

    HRESULT CompileShaderFromFile(PCWSTR pszFileName, PCSTR pszEntryPoint, PCSTR pszShaderModel, ID3DBlob** ppBlobOut)
    {
        HRESULT hr = S_OK;
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined (DEBUG) || defined(_DEBUG)
        dwShaderFlags |= D3DCOMPILE_DEBUG;
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ID3DBlob* pERrorBlob = nullptr;
        hr = D3DCompileFromFile(
            pszFileName,
            nullptr,
            nullptr,
            pszEntryPoint,
            pszShaderModel,
            dwShaderFlags,
            0,
            ppBlobOut,
            &pERrorBlob
        );

        if (FAILED(hr))
        {
            if (pERrorBlob)
            {
                OutputDebugStringA(
                    reinterpret_cast<PCSTR>(pERrorBlob->GetBufferPointer()));
                pERrorBlob->Release();
            }
            return hr;
        }
        if (pERrorBlob) pERrorBlob->Release();
        return S_OK;

  
    };

    void CleanupDevice()
    {
        if (g_pImmediateContext) g_pImmediateContext->ClearState();

        if (g_pVertexBuffer) g_pVertexBuffer->Release();
        if (g_pVertexLayout) g_pVertexLayout->Release();
        if (g_pVertexShader) g_pVertexShader->Release();
        if (g_pPixelShader) g_pPixelShader->Release();

        if (g_pRenderTargetView) g_pRenderTargetView->Release();
        if (g_pSwapChain1) g_pSwapChain1->Release();
        if (g_pSwapChain) g_pSwapChain->Release();
        if (g_pImmediateContext1) g_pImmediateContext1->Release();
        if (g_pImmediateContext) g_pImmediateContext->Release();
        if (g_pd3dDevice1) g_pd3dDevice1->Release();
        if (g_pd3dDevice) g_pd3dDevice->Release();

     
    }
    void Render()
    {
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);
        g_pSwapChain->Present(0, 0);
    }
 


    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_SIZE:
            break;
        case WM_CLOSE:
            if (MessageBox(hWnd, L"Exit?", L"Game Graphics Programming", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hWnd);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
        }
    }

   
}