using Hedron.Definition;

#nullable enable

namespace Hedron.Client
{
    [Module(Version: 0)]
    public class light_config_asset_0 : ILightConfig
    {
        public override ushort MaxPrimaryLight()
        {
            return 32;
        }

        public override ushort MaxProbes()
        {
            return 64;
        }
    }

    [Module(Version: 0)]
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