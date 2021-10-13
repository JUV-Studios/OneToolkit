#pragma once
#ifdef WinUI_3
#define Component OneToolkit::UI::WinUI
namespace Framework = Microsoft::UI::Xaml;
#else
#define Component OneToolkit::UI::Xaml
namespace Framework = Windows::UI::Xaml;
#endif