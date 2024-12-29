using Hedron.Feature;

namespace Hedron.Client
{
    [Feature(Version: 0)]
    public class light_config_asset_0 : ILightConfig
    {
        [FeatureParam] Platform platform;

        public override ushort MaxPrimaryLight()
        {
            return 32;
        }

        public override ushort MaxProbes()
        {
            return 64;
        }
    }

    [Feature(Version: 0)]
    public class light_config_asset_3 : ILightConfig
    {
        public override ushort MaxPrimaryLight()
        {
            return 144;
        }

        public override ushort MaxProbes()
        {
            return 44;
        }
    }
}