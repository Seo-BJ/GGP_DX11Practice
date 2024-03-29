#pragma once

#include "Common.h"

namespace library
{
    /*--------------------------------------------------------------------
      Forward declarations
    --------------------------------------------------------------------*/

    /*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
      Function: Render

      Summary:  Render the frame
    F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
    //void PrintHi();

  

    HRESULT InitWindow(_In_ HINSTANCE hInstance, _In_ INT nCmdShow);
    HRESULT InitDevice();
    
    HRESULT CompileShaderFromFile(
        _In_ PCWSTR pszFileName,
        _In_ PCSTR pszEntryPoint,
        _In_ PCSTR pszShaderModel,
        _Outptr_ ID3DBlob** ppBlobOut
    );

    void CleanupDevice();
    void Render();

    LRESULT CALLBACK WindowProc(
        _In_ HWND hWnd,
        _In_ UINT uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam
    );

    struct SimpleVertex
    {
        XMFLOAT3 Pos;
    };
}

