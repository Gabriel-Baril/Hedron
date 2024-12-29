using Hedron.Feature;
using System.ComponentModel;

namespace Hedron.Client
{
    [Feature(Version: 0)]
    public class light_config_asset_1 : ILightConfig
    {
        [FeatureParam] ushort param0;
        [FeatureParam] ushort param1;
        [FeatureParam] Platform Platform;

        public light_config_asset_1()
        {
        }

        public override ushort MaxPrimaryLight()
        {
            return (ushort)(32 + param0);
        }

        public override ushort MaxProbes()
        {
            return (ushort)(64 * param0);
        }

        public override void Construct()
        {
            if (Constructed)
            {
                return;
            }

            this.param0 = GetParam(BinderUtil.PARAM0_PARAM, default(ushort));
            this.param1 = GetParam("Param1", default(ushort));
            this.Platform = GetParam(BinderUtil.PLATFORM_PARAM, Platform.Window);
            base.Construct();
        }
    }
}