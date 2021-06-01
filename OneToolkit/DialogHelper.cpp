#include "pch.h"
#include "DialogHelper.h"
#include "UI.DialogHelper.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::OneToolkit::UI::implementation
{
	std::atomic_bool _IsDialogShown;

	bool DialogHelper::IsDialogShown() noexcept
	{
		return _IsDialogShown;
	}

	void DialogHelper::IsDialogShown(bool value) noexcept
	{
		_IsDialogShown = value;
	}

	IAsyncOperation<bool> DialogHelper::TryShowDialogAsync(ContentDialog dialog, ContentDialogPlacement placement, DialogDisplayCompletionAsyncHandler completionHandler) noexcept
	{
		if (!_IsDialogShown)
		{
			_IsDialogShown = true;
			auto result = co_await dialog.ShowAsync(placement);
			co_await completionHandler(result);
			_IsDialogShown = false;
			co_return true;
		}

		co_return false;
	}
}