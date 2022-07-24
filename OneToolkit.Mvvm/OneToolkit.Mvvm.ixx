// Code adapted from https://github.com/johanlindfors/MvvmCppWinRT.

module;

#include <functional>
#include <winrt/Windows.Foundation.h>
#if __has_include(<winrt/Microsoft.UI.h>)
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#else
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#endif

export module OneToolkit.Mvvm;

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
#if __has_include(<winrt/Microsoft.UI.h>)
using namespace Microsoft::UI::Xaml::Data;
using namespace Microsoft::UI::Xaml::Input;
#else
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
#endif

export namespace winrt::OneToolkit::Mvvm
{
	/// @brief Provides a base class for observable objects.
	template <typename Derived>
	struct ObservableObject
	{
	public:
		event_token PropertyChanged(PropertyChangedEventHandler const& handler)
		{
			return m_PropertyChanged.add(handler);
		}

		void PropertyChanged(event_token token) noexcept
		{
			m_PropertyChanged.remove(token);
		}
	protected:
		ObservableObject() = default;

		void RaisePropertyChanged(hstring const& propertyName)
		{
			if (!has_only_whitespaces(propertyName))
			{
				PropertyChangedEventArgs args{ propertyName };
				m_PropertyChanged(*static_cast<Derived*>(this), args);
			}
		}

		template <typename T>
		bool SetProperty(T& field, T newValue, hstring const& propertyName)
		{
			if (field != newValue)
			{
				field = newValue;
				RaisePropertyChanged(propertyName);
				return true;
			}

			return false;
		}
	private:
		event<PropertyChangedEventHandler> m_PropertyChanged;
	};

	template <typename Value, typename Owner, size_t FieldOffset, System::StringTemplateParameter PropertyName>
	struct ObservableProperty final
	{
	public:
		friend Owner;

		ObservableProperty(ObservableProperty&&) = delete;

		ObservableProperty(ObservableProperty const&) = delete;

		template <typename TValue = Value>
		ObservableProperty(TValue&& defaultValue = {}) : m_BackingField(std::forward<TValue>(defaultValue)) {}

		Value operator()() const noexcept
		{
			return m_BackingField;
		}

		template <typename TValue = Value>
		void operator()(TValue&& newValue)
		{
			reinterpret_cast<Owner*>(this - FieldOffset)->SetProperty(m_BackingField, newValue, PropertyName.Value);
		}
	private:
		Value m_BackingField;
	};

	struct RelayCommand : implements<RelayCommand, ICommand>
	{
	public:
		RelayCommand(std::function<void(IInspectable)> const& execute) : RelayCommand(execute, {}) {}

		RelayCommand(std::function<void(IInspectable)> const& execute, std::function<bool(IInspectable)> const& canExecute) : m_ExecuteDelegate(execute), m_CanExecuteDelegate(canExecute)
		{
			if (!execute) throw hresult_invalid_argument(L"execute");
		}

		event_token CanExecuteChanged(EventHandler<IInspectable> const& handler)
		{
			return m_CanExecuteChanged.add(handler);
		}

		void CanExecuteChanged(event_token const& token) noexcept
		{
			m_CanExecuteChanged.remove(token);
		}

		void RaiseCanExecuteChanged()
		{
			m_CanExecuteChanged(*this, nullptr);
		}

		bool CanExecute(IInspectable const& parameter)
		{
			if (m_CanExecuteDelegate)
			{
				bool canExecute = m_CanExecuteDelegate(parameter);
				if (m_PreviousCanExecute != canExecute)
				{
					m_PreviousCanExecute = canExecute;
					RaiseCanExecuteChanged();
				}
			}

			return m_PreviousCanExecute;
		}

		void Execute(IInspectable const& parameter)
		{
			m_ExecuteDelegate(parameter);
		}
	private:
		bool m_PreviousCanExecute = true;
		std::function<void(IInspectable)> m_ExecuteDelegate;
		std::function<bool(IInspectable)> m_CanExecuteDelegate;
		event<EventHandler<IInspectable>> m_CanExecuteChanged;
	};
}