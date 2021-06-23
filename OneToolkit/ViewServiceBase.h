#pragma once
#include <winrt/OneToolkit.UI.h>
#include <winrt/Windows.ApplicationModel.Core.h>

namespace winrt::OneToolkit::UI
{
	template <typename Derived>
	struct ViewServiceBase : implements<Derived, IViewServiceProvider, Lifecycle::IEquatable, Windows::UI::Xaml::Data::INotifyPropertyChanged, non_agile>, Mvvm::ObservableBase<Derived>, Lifecycle::Equatable<Derived>
	{
	public:
		bool IsDialogShown()
		{
			return unbox_value_or(Windows::ApplicationModel::Core::CoreApplication::Properties().TryLookup(DialogShownKey()), false);
		}

		void IsDialogShown(bool value)
		{
			if (IsDialogShown() != value)
			{
				static slim_mutex dialogShownLock;
				const slim_lock_guard lockGuard { dialogShownLock };
				Windows::ApplicationModel::Core::CoreApplication::Properties().Insert(DialogShownKey(), box_value(value));
			}
		}

		bool operator==(Windows::Foundation::IInspectable const& another) const noexcept
		{
			if (auto viewServiceProvider = another.try_as<IViewServiceProvider>()) return viewServiceProvider.WindowHandle() == static_cast<const Derived*>(this)->WindowHandle();
			else return false;
		}
	private:
		std::wstring m_DialogShownKey;

		inline std::wstring& DialogShownKey()
		{
			if (m_DialogShownKey.empty()) m_DialogShownKey = L"IsDialogShown-" + std::to_wstring(static_cast<Derived*>(this)->WindowHandle());
			return m_DialogShownKey;
		}
	};
}