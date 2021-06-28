using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using OneToolkit.Runtime;
using Microsoft.Toolkit.Uwp;

namespace OneToolkit.Showcase.Models
{
	public sealed record TypeGroup(string Name, IEnumerable<Type> Types)
	{
		private IEnumerable<TypeGroup> _Children;

		public IEnumerable<TypeGroup> Children => _Children ??= IsType ? null : _Children = Types.Select(type => new TypeGroup(type.Name, new[] { type }));

		public IEnumerable<TypeGroup> Classes
		{
			get
			{
				if (IsType)
				{
					return from type in Types.First().GetNestedTypes()
						   where type.IsClass
						   select new TypeGroup(type.Name, new[] { type });
				}
				else
				{
					return from type in Children
						   where type.Types.First().IsClass
						   select type;
				}
			}
		}

		public string DisplayName
		{
			get
			{
				if (IsType)
				{
					var type = Types.First();
					if (type.IsClass) return $"{Name} {"ClassText".GetLocalized()}";
					else return Name;
				}
				else
				{
					return $"{Name} {"NamespaceText".GetLocalized()}";
				}
			}
		}

		public bool IsType = Types.First().Name == Name;

		public Assembly Metadata => IsType ? Types.First().Assembly : null;

		public string DynamicLibrary => Metadata.GetDllFileName();

		public string Platform => Metadata.IsWinMD() ? "Windows" : ".NET Standard";
	}
}