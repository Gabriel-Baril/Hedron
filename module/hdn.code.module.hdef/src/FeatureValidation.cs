using Hedron.Feature;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
