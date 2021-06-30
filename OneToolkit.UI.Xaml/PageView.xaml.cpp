//
// PageView.xaml.cpp
// Implementation of the PageView class
//

#include "pch.h"
#include "PageView.xaml.h"
#include "DefaultContentTransition.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::UI;
using namespace OneToolkit::UI::Xaml::Controls;
using namespace OneToolkit::UI::Xaml::Media::Animation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

PageView::PageView()
{
	InitializeComponent();
}