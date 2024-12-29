using Hedron.Feature;

namespace HedronFeatureDescriptor
{
    public enum FeatureValidationMessageType
    {
        Info,
        Warning,
        Error,
    }

    public class FeatureValidationMessage
    {
        public FeatureValidationMessageType MessageType { get; set; }
        public IFeature Feature;

        public FeatureValidationMessage(FeatureValidationMessageType messageType, IFeature feature)
        {
            MessageType = messageType;
            Feature = feature;
        }
    }
}
