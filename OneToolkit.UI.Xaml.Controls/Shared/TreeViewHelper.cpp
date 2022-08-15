#include "pch.h"
#include "TreeViewHelper.g.h"
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt;
using namespace Windows::Foundation::Collections;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::OneToolkit::UI::Xaml::Controls
{
	namespace implementation
	{
		struct TreeViewHelper : TreeViewHelperT<TreeViewHelper>
		{
		public:
			TreeViewHelper() = delete;

			static IVectorView<TreeViewNode> GetAllNodes(TreeView const& treeView)
			{
				std::vector<TreeViewNode> allNodes;
				for (auto const& rootNode : treeView.RootNodes()) GetNodes(rootNode, allNodes);
				return single_threaded_vector(std::move(allNodes)).GetView();
			}
		private:
			static void GetNodes(TreeViewNode const& currentNode, std::vector<TreeViewNode>& allNodes)
			{
				if (currentNode)
				{
					allNodes.emplace_back(currentNode);
					for (auto const& childNode : currentNode.Children()) GetNodes(childNode, allNodes);
				}
			}
		};
	}

	namespace factory_implementation
	{
		struct TreeViewHelper : TreeViewHelperT<TreeViewHelper, implementation::TreeViewHelper>
		{
		};
	}
}

#include "TreeViewHelper.g.cpp"