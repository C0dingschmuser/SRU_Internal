#include "base.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Base::Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_ACTIVATEAPP)
    {

        if (Base::Data::init)
        {
            ImGui_ImplDX9_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
            Base::Data::init = false;

            Base::Init(false);
        }

        if (kiero::pDxDevice9)
        {
            kiero::pDxDevice9->Reset(&kiero::d3dpp);
        }
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
    
    if (ImGui::GetCurrentContext() != NULL)
    {
        ImGuiIO& io = ImGui::GetIO();

        if (io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            return TRUE;
        }
    }
    
    return CallWindowProc(Base::Data::oWndProc, hWnd, uMsg, wParam, lParam);
}