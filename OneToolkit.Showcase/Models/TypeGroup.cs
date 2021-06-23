using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using OneToolkit.Runtime;

namespace OneToolkit.Showcase.Models
{
	public sealed record TypeGroup(string Name, IEnumerable<Type> Types)
	{
		private IEnumerable<TypeGroup> _Children;

		public IEnumerable<TypeGroup> Children => _Children ??= IsType ? null : _Children = Types.Select(type => new TypeGroup(type.Name, new[] { type }));

		public string DisplayName => $"{Name} namespace";

		public bool IsType = Types.First().Name == Name;

		public Assembly Metadata => IsType ? Types.First().Assembly : null;

		public string DynamicLibrary => Metadata.GetDllFileName();

		public string Platform => Metadata.IsWinMD() ? "Windows" : ".NET Standard";
	}
}