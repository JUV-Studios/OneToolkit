using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using OneToolkit.Data.Text;

namespace OneToolkit.Dotnet.UnitTest
{
	[TestClass]
	public class GreetingHelperTest
	{
		[TestMethod]
		public void GetTimelyGreeting()
		{
			DisplayGreeting(DateTime.Parse("00:00:00"), GreetingType.GoodNight);
			DisplayGreeting(DateTime.Parse("04:00:00"), GreetingType.GoodMorning);
			DisplayGreeting(DateTime.Parse("12:00:00"), GreetingType.GoodMorning);
			DisplayGreeting(DateTime.Parse("12:01:00"), GreetingType.GoodAfternoon);
			DisplayGreeting(DateTime.Parse("16:00:00"), GreetingType.GoodAfternoon);
			DisplayGreeting(DateTime.Parse("21:00:00"), GreetingType.GoodEvening);
			DisplayGreeting(DateTime.Parse("22:00:00"), GreetingType.GoodNight);
		}

		private void DisplayGreeting(DateTime dateTime, GreetingType expectedType) => Assert.AreEqual(GreetingHelper.GetTimelyGreeting(dateTime), expectedType, $"Greeting type for {dateTime.ToLongTimeString()}");
	}
}