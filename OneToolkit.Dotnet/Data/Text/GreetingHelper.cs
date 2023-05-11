using System;

namespace OneToolkit.Data.Text
{
	/// <summary>
	/// Represents the type of a timely greeting.
	/// </summary>
#pragma warning disable CS1591
	public enum GreetingType
	{
		GoodMorning, GoodAfternoon, GoodEvening, GoodNight
	}
#pragma warning restore CS1591

	/// <summary>
	/// Provides static helper methods to deal with timely greetings.
	/// </summary>
	public static class GreetingHelper
	{
		/// <summary>
		/// Gets a suitable greeting message type based on the specified date and time.
		/// </summary>
		/// <param name="dateTime">The specified date and time.</param>
		/// <returns>An value from the GreetingType enumeration that represents the most accurate greeting for the specified date and time.</returns>
		public static GreetingType GetTimelyGreeting(this DateTime dateTime)
		{
			if (dateTime.Hour < 4 || dateTime.Hour > 21) return GreetingType.GoodNight;
			else if (dateTime.Hour <= 12) return dateTime.Hour == 12 && dateTime.Minute > 0 ? GreetingType.GoodAfternoon : GreetingType.GoodMorning;
			else if (dateTime.Hour <= 16) return GreetingType.GoodAfternoon;
			else return GreetingType.GoodEvening;
		}
	}
}