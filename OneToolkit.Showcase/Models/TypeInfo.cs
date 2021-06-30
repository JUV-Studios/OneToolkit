using System;
using Microsoft.Toolkit.Uwp;

namespace OneToolkit.Showcase.Models
{
	public sealed record TypeInfo(Type Type)
	{
		public string Name => Type.Name;

		public string Caption
		{
			get
			{
				if (Type.IsClass) return $"{Type.Name}, {"ClassText".GetLocalized()}";
				else return Type.Name;
			}
		}
	}
}
