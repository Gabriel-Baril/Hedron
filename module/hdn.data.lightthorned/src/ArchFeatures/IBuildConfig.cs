using System.Collections.Generic;

namespace Hedron.Feature
{
    [ArchFeature(Version: 0, Name: "buildconfig")]
    public abstract class IBuildConfig : IFeature
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
