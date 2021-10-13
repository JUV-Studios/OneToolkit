#include "pch.h"

using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

namespace Component::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TreeViewHelper sealed
	{
	public:
		static IVectorView<Microsoft::UI::Xaml::Controls::TreeViewNode^>^ GetAllNodes(Microsoft::UI::Xaml::Controls::TreeView^ treeView)
		{
			std::vector<Microsoft::UI::Xaml::Controls::TreeViewNode^> result;
			for (auto const& node : treeView->RootNodes) GetNodes(node, result);
			return ref new VectorView<Microsoft::UI::Xaml::Controls::TreeViewNode^>(std::move(result));
		}

	private:
		TreeViewHelper();

		static void GetNodes(Microsoft::UI::Xaml::Controls::TreeViewNode^ currentNode, std::vector<Microsoft::UI::Xaml::Controls::TreeViewNode^>& nodes)
		{
			if (currentNode)
			{
				nodes.emplace_back(currentNode);
				for (auto const& childNode : currentNode->Children) GetNodes(childNode, nodes);
			}
		}
	};
}