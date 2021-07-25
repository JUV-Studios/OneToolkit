//
// ContentFrame.xaml.h
// Declaration of the ContentFrame class
//

#pragma once
#include "ContentFrame.g.h"
#include "IContentTransition.h"

namespace OneToolkit::UI::Xaml::Controls
{
	ref class ContentFrame;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentFrameContentChangedEventArgs sealed
	{
	public:
		property Windows::UI::Xaml::UIElement^ OldContent
		{
			Windows::UI::Xaml::UIElement^ get()
			{
				return m_OldContent;
			}
		}

		property Windows::UI::Xaml::UIElement^ NewContent
		{
			Windows::UI::Xaml::UIElement^ get()
			{
				return m_NewContent;
			}
		}
	internal:
		ContentFrameContentChangedEventArgs(Windows::UI::Xaml::UIElement^ oldContent, Windows::UI::Xaml::UIElement^ newContent) : m_OldContent(oldContent), m_NewContent(newContent)
		{
		}
	private:
		Windows::UI::Xaml::UIElement^ m_OldContent;
		Windows::UI::Xaml::UIElement^ m_NewContent;
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void ContentFrameContentChangedEventHandler(ContentFrame^ sender, ContentFrameContentChangedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentFrame sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		ContentFrame();

		DeclareDependencyProperty(bool, SyncBackWithSystem);

		DeclareDependencyProperty(bool, IsBackStackEnabled);

		DeclareDependencyProperty(Media::Animation::IContentTransition^, ContentTransition);

		property bool CanGoBack
		{
			bool get();
		}

		event ContentFrameContentChangedEventHandler^ ContentChanged;

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		void GoBack();
		
		void ClearBackStack();

		void Navigate(Windows::UI::Xaml::UIElement^ content);

		Platform::Array<Windows::UI::Xaml::UIElement^>^ GetBackStack();
	private:
		std::stack<Windows::UI::Xaml::UIElement^> m_BackStack;
		Windows::Foundation::EventRegistrationToken m_BackToken;
		Windows::UI::Core::SystemNavigationManager^ m_NavigationManager = Windows::UI::Core::SystemNavigationManager::GetForCurrentView();
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void SetContent(Windows::UI::Xaml::UIElement^ content, NavigationType navigationType);
		void ContentPresenter_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ContentPresenter_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnBackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args);
	};
}