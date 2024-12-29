using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
