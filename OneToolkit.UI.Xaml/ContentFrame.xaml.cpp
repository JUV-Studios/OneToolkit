//
// ContentFrame.xaml.cpp
// Implementation of the ContentFrame class
//

#include "pch.h"
#include "ContentFrame.xaml.h"
#include "DefaultContentTransition.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace OneToolkit::UI::Xaml;
using namespace OneToolkit::UI::Xaml::Controls;
using namespace OneToolkit::UI::Xaml::Media::Animation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DefineDependencyProperty(bool, ContentFrame, SyncBackWithSystem, true);

DefineDependencyProperty(bool, ContentFrame, IsBackStackEnabled, true);

DefineDependencyProperty(IContentTransition^, ContentFrame, ContentTransition, ref new DefaultContentTransition);

ContentFrame::ContentFrame()
{
	InitializeComponent();
}

bool ContentFrame::CanGoBack::get()
{
	return m_BackStack.size() > 1;
}

void ContentFrame::GoBack()
{
	if (CanGoBack)
	{
		m_BackStack.pop();
		SetContent(m_BackStack.top(), NavigationType::Back);
		PropertyChanged(this, ref new PropertyChangedEventArgs("CanGoBack"));
	}
}

void ContentFrame::ClearBackStack()
{
	if (CanGoBack)
	{
		std::stack<UIElement^> temp;
		m_BackStack.swap(temp);
		m_BackStack.push(dynamic_cast<UIElement^>(Content));
		PropertyChanged(this, ref new PropertyChangedEventArgs("CanGoBack"));
	}
}

void ContentFrame::Navigate(UIElement^ content)
{
	if (Content != content)
	{
		SetContent(content, NavigationType::None);
		if (IsBackStackEnabled)
		{
			m_BackStack.push(content);
			PropertyChanged(this, ref new PropertyChangedEventArgs("CanGoBack"));
		}
	}
}

Array<UIElement^>^ ContentFrame::GetBackStack()
{
	auto result = ref new Array<UIElement^>(static_cast<uint32>(m_BackStack.size()));
	auto container = m_BackStack._Get_container();
	std::copy(container.end() - 1, container.begin(), result->end());
	return result;
}

void ContentFrame::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<ContentFrame^>(sender);
	if (e->Property == m_IsBackStackEnabledProperty)
	{
		if (!static_cast<bool>(e->NewValue)) target->ClearBackStack();
	}
	else if (e->Property == m_ContentTransitionProperty)
	{
		if (e->NewValue == nullptr)
		{
			if (auto defaultContentTransition = dynamic_cast<DefaultContentTransition^>(e->OldValue)) target->ContentTransition = defaultContentTransition;
			else target->ContentTransition = ref new DefaultContentTransition;
		}
	}
}

void ContentFrame::SetContent(Windows::UI::Xaml::UIElement^ content, NavigationType navigationType)
{
	auto args = ref new ContentFrameContentChangedEventArgs(dynamic_cast<UIElement^>(Content), content);
	ContentTransition->SetContent(this, content, navigationType);
	ContentChanged(this, args);
}

void ContentFrame::ContentPresenter_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	m_BackToken = m_NavigationManager->BackRequested += ref new EventHandler<BackRequestedEventArgs^>(this, &ContentFrame::OnBackRequested);
}

void ContentFrame::ContentPresenter_Unloaded(Object^ sender, RoutedEventArgs^ e)
{
	m_NavigationManager->BackRequested -= m_BackToken;
}

void ContentFrame::OnBackRequested(Object^ sender, BackRequestedEventArgs^ args)
{
	if (SyncBackWithSystem)
	{
		args->Handled = CanGoBack;
		GoBack();
	}
}