#include "pch.h"
#include "IconSourceHelper.h"
#include "KeyboardAcceleratorHelper.h"

using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Automation;
using namespace Windows::Foundation::Metadata;

namespace WUXC = Windows::UI::Xaml::Controls;
namespace MUXC = Microsoft::UI::Xaml::Controls;

namespace OneToolkit::UI::Controls
{
	[WebHostHidden]
	public ref class NavigationViewItemHelper sealed
	{
	public:
		static bool ApplyCommandResources(Object^ navViewItem, XamlUICommand^ command)
		{
			auto muxcItem = dynamic_cast<MUXC::NavigationViewItemBase^>(navViewItem);
			if (muxcItem != nullptr)
			{
				muxcItem->Content = command->Label;
				muxcItem->AccessKey = command->AccessKey;
				WUXC::ToolTipService::SetToolTip(muxcItem, command->Description);
				AutomationProperties::SetHelpText(muxcItem, command->Description);
				KeyboardAcceleratorHelper::SetKeyboardAccelerators(muxcItem, command->KeyboardAccelerators);
				auto classItem = dynamic_cast<MUXC::NavigationViewItem^>(muxcItem);
				if (classItem != nullptr) classItem->Icon = IconSourceHelper::CreateElement(command->IconSource);
				return true;
			}
			else
			{
				auto wuxcItem = dynamic_cast<WUXC::NavigationViewItemBase^>(navViewItem);
				if (wuxcItem != nullptr)
				{
					wuxcItem->Content = command->Label;
					wuxcItem->AccessKey = command->AccessKey;
					WUXC::ToolTipService::SetToolTip(wuxcItem, command->Description);
					AutomationProperties::SetHelpText(wuxcItem, command->Description);
					KeyboardAcceleratorHelper::SetKeyboardAccelerators(wuxcItem, command->KeyboardAccelerators);
					auto classItem = dynamic_cast<WUXC::NavigationViewItem^>(navViewItem);
					if (classItem != nullptr) classItem->Icon = IconSourceHelper::CreateElement(command->IconSource);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	private:
		NavigationViewItemHelper();
	};
}