using System;

namespace Hedron.Feature
{
    [FeatureParameterLess]
    [ArchFeature(Version: 0, Name: "parambinder")]
    public abstract class IParameterBinder : IFeature
    {
        public abstract dynamic Get(string ParamName, dynamic Default);

        public override byte[] SerializeData()
        {
            return Array.Empty<byte>();
        }
    }
}
