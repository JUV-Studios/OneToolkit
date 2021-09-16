#pragma once

namespace OneToolkit::UI::Xaml::Media::Animation
{
	interface struct IContentTransition;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentTransitionCompletedEventArgs sealed
	{
	public:
		property WUXC::ContentPresenter^ Presenter
		{
			WUXC::ContentPresenter^ get()
			{
				return m_Presenter;
			}
		}

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

		property OneToolkit::UI::NavigationType NavigationType
		{
			OneToolkit::UI::NavigationType get()
			{
				return m_NavigationType;
			}
		}
	internal:
		ContentTransitionCompletedEventArgs(WUXC::ContentPresenter^ presenter, Windows::UI::Xaml::UIElement^ oldContent, Windows::UI::Xaml::UIElement^ newContent,
			OneToolkit::UI::NavigationType navigationType) : m_Presenter(presenter), m_OldContent(oldContent), m_NewContent(newContent), m_NavigationType(navigationType) { }
	private:
		WUXC::ContentPresenter^ m_Presenter;
		Windows::UI::Xaml::UIElement^ m_OldContent;
		Windows::UI::Xaml::UIElement^ m_NewContent;
		OneToolkit::UI::NavigationType m_NavigationType;
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public delegate void ContentTransitionCompletedEventHandler(IContentTransition^ sender, ContentTransitionCompletedEventArgs^ args);

	[Windows::Foundation::Metadata::WebHostHidden]
	public interface struct IContentTransition
	{
		event ContentTransitionCompletedEventHandler^ ContentTransitionCompleted;
		void SetContent(WUXC::ContentPresenter^ presenter, Windows::UI::Xaml::UIElement^ content, NavigationType navigationType);
	};
}