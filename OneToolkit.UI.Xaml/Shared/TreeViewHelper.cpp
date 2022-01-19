#include "pch.h"

using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TreeViewHelper sealed
	{
	public:
		/// <summary>
		/// Gets all the nodes of a TreeView control in a single list including nested nodes.
		/// </summary>
		[Windows::Foundation::Metadata::DefaultOverload]
		static IVectorView<Windows::UI::Xaml::Controls::TreeViewNode^>^ GetAllNodes(Windows::UI::Xaml::Controls::TreeView^ treeView)
		{
			return GetAllNodes<Windows::UI::Xaml::Controls::TreeViewNode^>(treeView);
		}

		/// <summary>
		/// Gets all the nodes of a TreeView control in a single list including nested nodes.
		/// </summary>
		static IVectorView<Microsoft::UI::Xaml::Controls::TreeViewNode^>^ GetAllNodes(Microsoft::UI::Xaml::Controls::TreeView^ treeView)
		{
			return GetAllNodes<Microsoft::UI::Xaml::Controls::TreeViewNode^>(treeView);
		}
	private:
		TreeViewHelper();

		template <typename TreeViewNode, typename TreeView>
		static IVectorView<TreeViewNode>^ GetAllNodes(TreeView treeView)
		{
			std::vector<TreeViewNode> result;
			for (auto const& node : treeView->RootNodes) GetNodes<TreeViewNode>(node, result);
			return ref new VectorView<TreeViewNode>(std::move(result));
		}

		template <typename TreeViewNode>
		static void GetNodes(TreeViewNode currentNode, std::vector<TreeViewNode>& nodes)
		{
			if (currentNode)
			{
				nodes.emplace_back(currentNode);
				for (auto const& childNode : currentNode->Children) GetNodes<TreeViewNode>(childNode, nodes);
			}
		}
	};
}