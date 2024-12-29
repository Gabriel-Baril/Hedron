using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hedron.Feature
{
    [FeatureParameterLess]
    [ArchFeature(Version: 0, Name: "parambinder")]
    public abstract class IParameterBinder : IFeature
    {
        public abstract dynamic Get(string ParamName, dynamic Default);

        public override byte[] SerializeData()
        {
            return Array.Empty<byte>();
        }
    }
}
