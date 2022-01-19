using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using OneToolkit.DocsCore;

namespace OneToolkit.Dotnet.UnitTest
{
	[TestClass]
	public class TypeInfoTest
	{
		[TestMethod]
		public void GetFullName()
		{
			Assert.AreEqual(DisplayFullName(typeof(char), "C#"), "char");
			Assert.AreEqual(DisplayFullName(typeof(sbyte), "C#"), "sbyte");
			Assert.AreEqual(DisplayFullName(typeof(byte), "C#"), "byte");
			Assert.AreEqual(DisplayFullName(typeof(short), "C#"), "short");
			Assert.AreEqual(DisplayFullName(typeof(ushort), "C#"), "ushort");
			Assert.AreEqual(DisplayFullName(typeof(int), "C#"), "int");
			Assert.AreEqual(DisplayFullName(typeof(uint), "C#"), "uint");
			Assert.AreEqual(DisplayFullName(typeof(long), "C#"), "long");
			Assert.AreEqual(DisplayFullName(typeof(ulong), "C#"), "ulong");
			Assert.AreEqual(DisplayFullName(typeof(float), "C#"), "float");
			Assert.AreEqual(DisplayFullName(typeof(double), "C#"), "double");
			Assert.AreEqual(DisplayFullName(typeof(decimal), "C#"), "decimal");
			Assert.AreEqual(DisplayFullName(typeof(nint), "C#"), "ninit");
			Assert.AreEqual(DisplayFullName(typeof(nuint), "C#"), "nuint");
			Assert.AreEqual(DisplayFullName(typeof(object), "C#"), "object");
			Assert.AreEqual(DisplayFullName(typeof(string), "C#"), "string");
		}

		public string DisplayFullName(Type type, string codeLanguage) => TypeInfo.GetInstance(type).GetFullName(codeLanguage);
	}
}
