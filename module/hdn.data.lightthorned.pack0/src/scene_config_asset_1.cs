using Hedron.Feature;

#nullable enable

namespace Hedron.Client
{
    [Feature(Version: 0)]
    public class scene_config_asset_1 : ISceneConfig
    {
        // Parameters
        ushort param0;

        // Caching
        ILightConfig? lightConfig;

        public scene_config_asset_1()
        {
            this.param0 = 0;// param0;
        }

        public override bool ShouldGenerateProxyGeo()
        {
            return param0 > 5;
        }

        public override ILightConfig? LightConfig()
        {
            return lightConfig;
        }

        public override void Construct()
        {
            if (Constructed)
            {
                return;
            }

            if (param0 > 5)
            {
                lightConfig = new light_config_asset_1();
            }
            else
            {
                lightConfig = new light_config_asset_0();
            }

            base.Construct();
        }
    }
}