using System;
using System.Linq;
using System.Collections.Generic;
using System.Reflection;
using OneToolkit.Lifecycle;
using OneToolkit.Mvvm;
using OneToolkit.Showcase.Models;
using OneToolkit.UI.Xaml.Controls;
using MUXC = Microsoft.UI.Xaml.Controls;
using OneToolkit.Showcase.Views;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class ReferenceViewModel : ObservableBase
	{
		public static readonly Assembly[] ToolkitAssemblies = new[]
		{
			typeof(IAsyncClosable).Assembly, typeof(PageView).Assembly, typeof(ObservableBase).Assembly
		};

		public static readonly string[] ExcludedNamespaces = new[]
		{
			"OneToolkit.UI.Xaml.OneToolkit_UI_Xaml_XamlTypeInfo", "System.Runtime.CompilerServices", "Microsoft.CodeAnalysis"
		};

		public static IEnumerable<TypeGroup> FoundTypes;

		public static IEnumerable<PageViewFrameItem> BindableNamespaces
		{
			get => from nameSpace in FoundTypes 
				   select new PageViewFrameItem { Label = nameSpace.Caption, SourcePageType = typeof(NamespacePage), Parameter = nameSpace };
		}

		public static void Initialize()
		{
			FoundTypes ??= from assembly in ToolkitAssemblies
						   from type in assembly.GetTypes()
						   where !ExcludedNamespaces.Contains(type.Namespace) && !type.Name.StartsWith("__") && !type.Name.EndsWith("Statics") && !type.Name.EndsWith("Factory")
						   group type by type.Namespace into types
						   select new TypeGroup(types.Key, types);
		}
	}
}
