using System;

namespace Hedron.Definition
{
    [Definition(Version: 0, Name: "parambinder")]
    public abstract class IParameterBinder : IDefinition
    {
        public abstract dynamic Get(string ParamName, dynamic Default);

        public override byte[] SerializeData()
        {
            return Array.Empty<byte>();
        }
    }
}
