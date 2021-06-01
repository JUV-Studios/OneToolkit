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
		static bool ApplyCommandResources(WUXC::ContentControl^ navViewItem, XamlUICommand^ command)
		{
			if (auto muxcItem = dynamic_cast<MUXC::NavigationViewItemBase^>(navViewItem))
			{
				muxcItem->Content = command->Label;
				muxcItem->AccessKey = command->AccessKey;
				WUXC::ToolTipService::SetToolTip(muxcItem, command->Description);
				AutomationProperties::SetHelpText(muxcItem, command->Description);
				KeyboardAcceleratorHelper::SetKeyboardAccelerators(muxcItem, command->KeyboardAccelerators);
				if (auto classItem = dynamic_cast<MUXC::NavigationViewItem^>(muxcItem)) classItem->Icon = IconSourceHelper::CreateElement(command->IconSource);
				return true;
			}
			else if (auto wuxcItem = dynamic_cast<WUXC::NavigationViewItemBase^>(navViewItem))
			{
				wuxcItem->Content = command->Label;
				wuxcItem->AccessKey = command->AccessKey;
				WUXC::ToolTipService::SetToolTip(wuxcItem, command->Description);
				AutomationProperties::SetHelpText(wuxcItem, command->Description);
				KeyboardAcceleratorHelper::SetKeyboardAccelerators(wuxcItem, command->KeyboardAccelerators);
				if (auto classItem = dynamic_cast<WUXC::NavigationViewItem^>(navViewItem)) classItem->Icon = IconSourceHelper::CreateElement(command->IconSource);
				return true;
			}
			else
			{
				return false;
			}
		}
	private:
		NavigationViewItemHelper();
	};
}