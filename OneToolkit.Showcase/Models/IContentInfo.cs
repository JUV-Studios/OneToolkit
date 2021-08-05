using System;
using System.Collections.Generic;

namespace OneToolkit.Showcase.Models
{
	public interface IContentInfo
	{
		public string Name { get; }

		public string DisplayName { get; }

		public string Caption { get; }

		public IContentInfo Parent { get; }

		public IEnumerable<IContentInfo> Children { get; }

		public Type PageType { get; }
	}
}