using Google.FlatBuffers;
using System.Collections.Generic;

namespace Hedron.Definition
{
    public static class ILightConfigUtil
    {
        public static Offset<LightConfig> ConstructLightConfig(FlatBufferBuilder builder, ILightConfig definition)
        {
            LightConfig.StartLightConfig(builder);
            LightConfig.AddMaxPrimaryLight(builder, definition.MaxPrimaryLight());
            LightConfig.AddMaxProbes(builder, definition.MaxProbes());
            return LightConfig.EndLightConfig(builder);
        }

        public class MaxProbeLimitError : DefinitionValidationMessage
        {
            public ushort given;
            public ushort max;

            public MaxProbeLimitError(IDefinition definition, ushort given, ushort max) : base(DefinitionValidationMessageType.Error, definition)
            {
                this.max = max;
                this.given = given;
            }

            public override string ToString()
            {
                return $"Max probe limit of '{max}' hit; {given} > {max}.";
            }
        }

        public class MaxPrimaryLightLimitError : DefinitionValidationMessage
        {
            public ushort given;
            public ushort max;

            public MaxPrimaryLightLimitError(IDefinition definition, ushort given, ushort max) : base(DefinitionValidationMessageType.Error, definition)
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

    [Definition(Version: 0, Name: "lightconfig")]
    public abstract class ILightConfig : IDefinition
    {
        public const ushort MAX_PROBES_LIMIT = 128;
        public const ushort MAX_PRIMARY_LIGHT_LIMIT = 128;

        public abstract ushort MaxPrimaryLight();
        public abstract ushort MaxProbes();

        public sealed override List<DefinitionValidationMessage> Validate()
        {
            List<DefinitionValidationMessage> errors = new List<DefinitionValidationMessage>();

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
