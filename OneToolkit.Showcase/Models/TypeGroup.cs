using System;
using System.Linq;
using System.Collections.Generic;
using Microsoft.Toolkit.Uwp;

namespace OneToolkit.Showcase.Models
{
	public sealed record TypeGroup(string Name, IEnumerable<Type> Types)
	{
		public IEnumerable<TypeInfo> Classes
		{
			get
			{
				
				return from type in Types
					   where type.IsClass
					   select new TypeInfo(type);
			}
		}

		public string Caption => $"{Name} {"NamespaceText".GetLocalized()}";
	}
}