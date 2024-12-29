namespace Hedron.Feature
{
    [ArchFeature(Version: 0, Name: "scene")]
    public abstract class IScene : IFeature
    {
        public abstract ISceneConfig SceneConfig();

        public override void RegisterDependencies()
        {
            base.RegisterDependencies();
            AddDependency(SceneConfig());
        }
    }
}
