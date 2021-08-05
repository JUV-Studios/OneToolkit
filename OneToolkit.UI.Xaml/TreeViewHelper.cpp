#include "pch.h"

using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TreeViewHelper sealed
	{
	public:
		static IVectorView<MUXC::TreeViewNode^>^ GetAllNodes(MUXC::TreeView^ treeView)
		{
			std::vector<MUXC::TreeViewNode^> result;
			for (auto const& node : treeView->RootNodes) GetNodes(node, result);
			return ref new VectorView<MUXC::TreeViewNode^>(std::move(result));
		}

	private:
		TreeViewHelper();

		static void GetNodes(MUXC::TreeViewNode^ currentNode, std::vector<MUXC::TreeViewNode^>& nodes)
		{
			if (currentNode)
			{
				nodes.emplace_back(currentNode);
				for (auto const& childNode : currentNode->Children) GetNodes(childNode, nodes);
			}
		}
	};
}