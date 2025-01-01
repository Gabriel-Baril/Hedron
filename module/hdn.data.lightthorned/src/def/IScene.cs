namespace Hedron.Definition
{
    [Definition(Version: 0, Name: "scene")]
    public abstract class IScene : IDefinition
    {
        public abstract ISceneConfig SceneConfig();

        public override void RegisterDependencies()
        {
            base.RegisterDependencies();
            AddDependency(SceneConfig());
        }
    }
}
