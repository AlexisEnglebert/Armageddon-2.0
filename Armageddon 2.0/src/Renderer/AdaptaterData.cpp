#include "AdaptaterData.h"
std::vector<Adpatater> AdaptaterReader::adaptaters;

std::vector<Adpatater> AdaptaterReader::GetAdaptaterData()
{
    if (adaptaters.size() > 0)  // L'array a déjà un élément ce qui veut dire quelle est déjà initialisée
    {
        return adaptaters;
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
    HRESULT handleR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
    if (FAILED(handleR))
    {
        Armageddon::Log::GetLogger()->error("ERREUR CRITIQUE, Handle to IDXGIFactory failed");
        exit(-1);
    }
    IDXGIAdapter* pAdapter;
    UINT Index = 0;
    while (SUCCEEDED(pFactory->EnumAdapters(Index, &pAdapter)))
    {
        adaptaters.push_back(Adpatater(pAdapter));
        Index++;

    }
    return adaptaters;
}

Adpatater::Adpatater(IDXGIAdapter* ptrAdaptater)
{
    this->ptrAdaptater = ptrAdaptater;
    HRESULT hr = ptrAdaptater->GetDesc(&this->Description);
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("Aucune Carte Graphique compatible de trouvé");
        exit(-1);
    }
}
