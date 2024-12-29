using Hedron.Feature;
using System.Collections.Generic;

#nullable enable

namespace Hedron.Client
{
    [Feature(Version: 0)]
    public class param_binder_test : IParameterBinder
    {
        Dictionary<string, dynamic> Params = new Dictionary<string, dynamic>();

        public override void Construct()
        {
            if (Constructed)
            {
                return;
            }

            Params.Add(BinderUtil.PLATFORM_PARAM, Platform.Window);
            Params.Add(BinderUtil.PARAM0_PARAM, (ushort)2);
            base.Construct();
        }

        public override dynamic Get(string ParamName, dynamic Default)
        {
            if (Params.TryGetValue(ParamName, out dynamic? value))
            {
                return value;
            }
            return Default;
        }
    }
}
