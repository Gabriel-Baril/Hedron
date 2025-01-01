using System;

namespace Hedron.Definition
{
    public class ModuleAttribute : Attribute
    {
        public uint Version { get; }

        public ModuleAttribute(uint Version)
        {
            this.Version = Version;
        }
    }

    public class DefinitionAttribute : Attribute
    {
        public uint Version { get; }
        public string Name { get; }

        public DefinitionAttribute(uint Version, string Name)
        {
            this.Version = Version;
            this.Name = Name;
        }
    }

    public class DefinitionTemplateAttribute : Attribute
    {
        public DefinitionTemplateAttribute()
        {

        }
    }
}
