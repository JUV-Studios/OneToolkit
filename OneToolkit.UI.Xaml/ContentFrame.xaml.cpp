//
// ContentFrame.xaml.cpp
// Implementation of the ContentFrame class
//

#include "pch.h"
#include "ContentFrame.xaml.h"
#include "SuppressContentTransition.h"

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
using namespace OneToolkit::UI::Xaml::Controls;
using namespace OneToolkit::UI::Xaml::Media::Animation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

DeclareDependencyProperty(bool, ContentFrame, SyncBackWithSystem, true);

DeclareDependencyProperty(bool, ContentFrame, IsNavigationStackEnabled, true);

DeclareDependencyProperty(IContentTransition, ContentFrame, ContentTransition, ref new SuppressContentTransition);

ContentFrame::ContentFrame()
{
	InitializeComponent();
}

bool ContentFrame::CanGoBack::get()
{
	return IsNavigationStackEnabled && m_BackStack.size() > 1;
}

bool ContentFrame::CanGoForward::get()
{
	return IsNavigationStackEnabled && !m_ForwardStack.empty();
}

bool ContentFrame::SyncBackWithSystem::get()
{
	return static_cast<bool>(GetValue(m_SyncBackWithSystemProperty));
}

void ContentFrame::SyncBackWithSystem::set(bool value)
{
	SetValue(m_SyncBackWithSystemProperty, value);
}

bool ContentFrame::IsNavigationStackEnabled::get()
{
	return static_cast<bool>(GetValue(m_IsNavigationStackEnabledProperty));
}

void ContentFrame::IsNavigationStackEnabled::set(bool value)
{
	SetValue(m_IsNavigationStackEnabledProperty, value);
}

IContentTransition^ ContentFrame::ContentTransition::get()
{
	return dynamic_cast<IContentTransition^>(GetValue(m_ContentTransitionProperty));
}

void ContentFrame::ContentTransition::set(IContentTransition^ value)
{
	if (value == nullptr && dynamic_cast<SuppressContentTransition^>(GetValue(m_ContentTransitionProperty)) == nullptr) value = ref new SuppressContentTransition();
	SetValue(m_ContentTransitionProperty, value);
}

void ContentFrame::GoBack()
{
	if (CanGoBack)
	{
		m_ForwardStack.push(m_BackStack.top());
		m_BackStack.pop();
		ContentTransition->SetContent(this, m_BackStack.top(), NavigationType::Back);
		Raise();
	}
}

void ContentFrame::GoForward()
{
	if (CanGoForward)
	{
		ContentTransition->SetContent(this, m_ForwardStack.top(), NavigationType::Forward);
		m_ForwardStack.pop();
		Raise();
	}
}

void ContentFrame::Navigate(UIElement^ content)
{
	if (Content != content)
	{
		if (IsNavigationStackEnabled) m_BackStack.push(content);
		ContentTransition->SetContent(this, content, NavigationType::None);
		Raise();
	}
}

void ContentFrame::NavigateToType(TypeName typeName)
{
	Object^ result;
	auto hr = RoActivateInstance(reinterpret_cast<HSTRING>(typeName.Name), reinterpret_cast<IInspectable**>(&result));
	if (FAILED(hr)) throw ref new Exception(hr);
	Navigate(static_cast<UIElement^>(result));
}

void ContentFrame::Clear(std::stack<UIElement^>& stack)
{
	std::stack<UIElement^> temp;
	stack.swap(temp);
}

void ContentFrame::DependencyPropertyChanged(DependencyObject^ sender, DependencyPropertyChangedEventArgs^ e)
{
	auto contentFrame = dynamic_cast<ContentFrame^>(sender);
	if (e->Property == m_IsNavigationStackEnabledProperty)
	{
		if (static_cast<bool>(e->OldValue) && !static_cast<bool>(e->NewValue))
		{
			Clear(contentFrame->m_BackStack);
			Clear(contentFrame->m_ForwardStack);
		}
	}

	contentFrame->Raise();
}

void ContentFrame::Raise()
{
	PropertyChanged(this, ref new PropertyChangedEventArgs("Content"));
	PropertyChanged(this, ref new PropertyChangedEventArgs("CanGoBack"));
	PropertyChanged(this, ref new PropertyChangedEventArgs("CanGoForward"));
}

void ContentFrame::ContentPresenter_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	m_BackRequestedToken = SystemNavigationManager::GetForCurrentView()->BackRequested += ref new EventHandler<BackRequestedEventArgs^>(this, &ContentFrame::OnBackRequested);
}

void ContentFrame::ContentPresenter_Unloaded(Object^ sender, RoutedEventArgs^ e)
{
	SystemNavigationManager::GetForCurrentView()->BackRequested -= m_BackRequestedToken;
}

void ContentFrame::OnBackRequested(Object^ sender, BackRequestedEventArgs^ args)
{
	if (SyncBackWithSystem && CanGoBack)
	{
		args->Handled = true;
		GoBack();
	}
}