using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hedron.Client
{
    public enum Platform
    {
        Window,
        Linux,
        Mac
    }

    public static class BinderUtil
    {
        public const string PLATFORM_PARAM = "Platform";
        public const string PARAM0_PARAM = "Param0";
    }
}
