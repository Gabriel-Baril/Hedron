using Hedron.Definition;

namespace Hedron.Client
{
    [Module(Version: 1)]
    public class scene_config_asset_0 : ISceneConfig
    {
        public override bool ShouldGenerateProxyGeo()
        {
            return false;
        }

        public override ILightConfig LightConfig()
        {
            return new light_config_asset_2();
        }
    }
}
