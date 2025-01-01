using System;
using System.Text;

#nullable enable

namespace Hedron.Definition
{
    public static class TypeUtil
    {
        public static string GetTypeChain(dynamic input_)
        {
            const string dlm = ".";
            Type? t = null;
            StringBuilder rv = new StringBuilder();

            t = input_.GetType();
            while (true)
            {
                if (t == null) break;
                rv.AppendFormat("{0}{1}", dlm, t.Name);
                t = t.BaseType;
            }

            return rv.ToString()[1..];
        }
    }
}