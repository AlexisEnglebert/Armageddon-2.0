#pragma once
#include "../../Macros.h"
#if WINDOWS
    #include <dxgi.h>
    #include <d3d11.h>
    #include <wrl\client.h>
    
    #include "backends/imgui_impl_win32.h"
    #include "backends/imgui_impl_dx11.h"

    #pragma comment(lib, "dxgi.lib")
    

#endif

namespace Armageddon
{
    class DirectX11Renderer
    {
        public: 
            DirectX11Renderer(){};
            ~DirectX11Renderer(){};
        	bool Init(HWND hwnd, float width, float height);

        private:

        	static Microsoft::WRL::ComPtr < ID3D11BlendState>		AlphaBlendState;
		    static Microsoft::WRL::ComPtr < ID3D11BlendState>		DefaultBlendState;
            
            static D3D11_VIEWPORT ViewPort;

            	/*DephtStencil*/
            Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DephtStencilView;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DephtStencilState;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> DephtStencilBuffer;

        	void CreateViewPort(float width, float height);
            bool InitSwapChain(HWND& hwnd);
            bool CreateDephtStencilBuffer(int width, int height);
            void ResetDephtStencileBuffer();
            void CreateRenderTargetView(float width, float height);
            void CleanRenderTargetView();

            void CreateRasterizer(D3D11_RASTERIZER_DESC rDesc);
            void ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc);
            void CreateAlphaBlendState();
            void CreateDefaultBlendState();
            
    };
}