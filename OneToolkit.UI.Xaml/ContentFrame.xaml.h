//
// ContentFrame.xaml.h
// Declaration of the ContentFrame class
//

#pragma once
#include "ContentFrame.g.h"
#include "IContentTransition.h"

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentFrame sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		ContentFrame();

		property bool CanGoBack
		{
			bool get();
		}

		property bool CanGoForward
		{
			bool get();
		}

		property bool SyncBackWithSystem
		{
			bool get();
			void set(bool value);
		}

		property bool IsNavigationStackEnabled
		{
			bool get();
			void set(bool value);
		}

		property Media::Animation::IContentTransition^ ContentTransition
		{
			Media::Animation::IContentTransition^ get();
			void set(Media::Animation::IContentTransition^ value);
		}

		static property Windows::UI::Xaml::DependencyProperty^ ContentTransitionProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ SyncBackWithSystemProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		static property Windows::UI::Xaml::DependencyProperty^ IsNavigationStackEnabledProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get();
		}

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		void GoBack();

		void GoForward();

		void Navigate(Windows::UI::Xaml::UIElement^ content);

		void NavigateToType(Windows::UI::Xaml::Interop::TypeName typeName);
	private:
		std::stack<Windows::UI::Xaml::UIElement^> m_BackStack;
		std::stack<Windows::UI::Xaml::UIElement^> m_ForwardStack;
		Windows::Foundation::EventRegistrationToken m_BackRequestedToken;
		static Windows::UI::Xaml::DependencyProperty^ m_ContentTransitionProperty;
		static Windows::UI::Xaml::DependencyProperty^ m_SyncBackWithSystemProperty;
		static Windows::UI::Xaml::DependencyProperty^ m_IsNavigationStackEnabledProperty;
		static void Clear(std::stack<Windows::UI::Xaml::UIElement^>& stack);
		static void DependencyPropertyChanged(Windows::UI::Xaml::DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ e);
		void Raise();
		void ContentPresenter_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ContentPresenter_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnBackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args);
	};
}
