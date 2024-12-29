using Hedron.Feature;

namespace Hedron.Client
{
    [FeatureTemplate]
    public class SimpleLightConfigTemplate : ILightConfig
    {
        ushort maxPrimaryLight;
        ushort maxProbes;

        public SimpleLightConfigTemplate(ushort maxPrimaryLight, ushort maxProbes)
        {
            this.maxPrimaryLight = maxPrimaryLight;
            this.maxProbes = maxProbes;
        }

        public override ushort MaxPrimaryLight()
        {
            return this.maxPrimaryLight;
        }

        public override ushort MaxProbes()
        {
            return this.maxProbes;
        }
    }
}
