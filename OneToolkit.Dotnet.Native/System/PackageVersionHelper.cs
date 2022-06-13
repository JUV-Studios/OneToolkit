using System;
using Windows.ApplicationModel;

namespace OneToolkit.System
{
	public static class PackageVersionExtensions
	{
		public static Version AsVersion(this PackageVersion packageVersion) => new Version(packageVersion.Major, packageVersion.Minor, packageVersion.Build, packageVersion.Revision);

		public static PackageVersion AsPackageVersion(this Version version) => new()
		{
			Major = Convert.ToUInt16(version.Major),
			Minor = Convert.ToUInt16(version.Minor),
			Build = Convert.ToUInt16(version.Build),
			Revision = Convert.ToUInt16(version.Revision)
		};
	}
}