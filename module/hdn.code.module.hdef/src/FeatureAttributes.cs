using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hedron.Feature
{
    public class FeatureAttribute : Attribute
    {
        public uint Version { get; }

        public FeatureAttribute(uint Version)
        {
            this.Version = Version;
        }
    }

    public class ArchFeatureAttribute : Attribute
    {
        public uint Version { get; }
        public string Name { get; }

        public ArchFeatureAttribute(uint Version, string Name)
        {
            this.Version = Version;
            this.Name = Name;
        }
    }

    public class FeatureTemplateAttribute : Attribute
    {
        public FeatureTemplateAttribute()
        {

        }
    }

    public class FeatureParam : Attribute
    {
        public string ExposedName { get; }
        public FeatureParam(string ExposedName = "")
        {
            this.ExposedName = ExposedName;
        }
    }

    public class FeatureParameterLessAttribute : Attribute
    {
        public FeatureParameterLessAttribute()
        {

        }
    }
}
