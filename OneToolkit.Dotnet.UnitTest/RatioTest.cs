using Microsoft.VisualStudio.TestTools.UnitTesting;
using OneToolkit.Numerics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OneToolkit.Dotnet.UnitTest
{
	[TestClass]
	public class RatioTest
	{
		[TestMethod]
		public void Simplify()
		{
			Assert.AreEqual(new Ratio(2, 4).Simplify(), new Ratio(1, 2));
		}
	}
}