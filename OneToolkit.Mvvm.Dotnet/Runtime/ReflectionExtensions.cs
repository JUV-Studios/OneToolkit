using System;
using System.IO;
using System.Reflection;

namespace OneToolkit.Runtime
{
	public static class ReflectionExtensions
	{
		/// <summary>
		/// Gets whether an assembly represents a Windows Runtime metadata file.
		/// </summary>
		public static bool IsWinMD(this Assembly assembly) => assembly.GetName().ContentType == AssemblyContentType.WindowsRuntime;

		/// <summary>
		/// Gets the implementation file name of an assembly.
		/// </summary>
		public static string GetDllFileName(this Assembly assembly)
		{
			if (assembly == null) return string.Empty;
			else if (assembly.IsWinMD())
			{
				var dllFileName = assembly.Location.Replace(".winmd", ".dll");
				return File.Exists(dllFileName) ? Path.GetFileName(dllFileName) : assembly.ManifestModule.Name;
			}
			else return assembly.ManifestModule.Name;
		}
	}
}