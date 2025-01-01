using System.Collections.Generic;

namespace Hedron.Definition
{
    [Definition(Version: 0, Name: "buildconfig")]
    public abstract class IBuildConfig : IDefinition
    {
        public abstract List<IScene> SceneToBuild();

        public override void RegisterDependencies()
        {
            base.RegisterDependencies();
            foreach (IScene scene in SceneToBuild())
            {
                AddDependency(scene);
            }
        }
    }
}
