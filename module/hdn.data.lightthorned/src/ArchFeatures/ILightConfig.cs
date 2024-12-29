using Google.FlatBuffers;
using HedronFeatureDescriptor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hedron.Feature
{
    public static class ILightConfigUtil
    {
        public static Offset<LightConfig> ConstructLightConfig(FlatBufferBuilder builder, ILightConfig feature)
        {
            LightConfig.StartLightConfig(builder);
            LightConfig.AddMaxPrimaryLight(builder, feature.MaxPrimaryLight());
            LightConfig.AddMaxProbes(builder, feature.MaxProbes());
            return LightConfig.EndLightConfig(builder);
        }

        public class MaxProbeLimitError : FeatureValidationMessage
        {
            public ushort given;
            public ushort max;

            public MaxProbeLimitError(IFeature feature, ushort given, ushort max) : base(FeatureValidationMessageType.Error, feature)
            {
                this.max = max;
                this.given = given;
            }

            public override string ToString()
            {
                return $"Max probe limit of '{max}' hit; {given} > {max}.";
            }
        }

        public class MaxPrimaryLightLimitError : FeatureValidationMessage
        {
            public ushort given;
            public ushort max;

            public MaxPrimaryLightLimitError(IFeature feature, ushort given, ushort max) : base(FeatureValidationMessageType.Error, feature)
            {
                this.max = max;
                this.given = given;
            }

            public override string ToString()
            {
                return $"Max primary light limit of '{max}' hit; {given} > {max}.";
            }
        }
    }

    [ArchFeature(Version: 0, Name: "lightconfig")]
    public abstract class ILightConfig : IFeature
    {
        public const ushort MAX_PROBES_LIMIT = 128;
        public const ushort MAX_PRIMARY_LIGHT_LIMIT = 128;

        public abstract ushort MaxPrimaryLight();
        public abstract ushort MaxProbes();

        public sealed override List<FeatureValidationMessage> Validate()
        {
            List<FeatureValidationMessage> errors = new List<FeatureValidationMessage>();

            ushort maxProbes = MaxProbes();
            ushort maxPrimaryLight = MaxPrimaryLight();

            if (maxProbes > MAX_PROBES_LIMIT)
            {
                errors.Add(new ILightConfigUtil.MaxProbeLimitError(this, maxProbes, MAX_PROBES_LIMIT));
            }

            if (maxPrimaryLight > MAX_PRIMARY_LIGHT_LIMIT)
            {
                errors.Add(new ILightConfigUtil.MaxPrimaryLightLimitError(this, maxPrimaryLight, MAX_PROBES_LIMIT));
            }

            return errors;
        }

        public override byte[] SerializeData()
        {
            FlatBufferBuilder builder = new FlatBufferBuilder(1024);
            var offset = ILightConfigUtil.ConstructLightConfig(builder, this);
            builder.Finish(offset.Value);
            return builder.SizedByteArray();
        }
    }
}
