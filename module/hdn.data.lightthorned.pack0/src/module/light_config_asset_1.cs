using Hedron.Definition;

namespace Hedron.Client
{
    [Module(Version: 0)]
    public class light_config_asset_1 : ILightConfig
    {
        ushort param0 = 0;

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

            base.Construct();
        }
    }
}