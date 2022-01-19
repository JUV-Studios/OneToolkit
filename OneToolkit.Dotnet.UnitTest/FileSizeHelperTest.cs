using Microsoft.VisualStudio.TestTools.UnitTesting;
using OneToolkit.Storage;

namespace OneToolkit.Dotnet.UnitTest
{
	[TestClass]
	public class FileSizeHelperTest
	{
		[TestMethod]
		public void Stringify()
		{
			Assert.AreEqual(FileSizeHelper.Stringify(0, true), "0 B");
			Assert.AreEqual(FileSizeHelper.Stringify(0, false), "0 B");
		}
	}
}
