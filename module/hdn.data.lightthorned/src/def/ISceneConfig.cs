using Google.FlatBuffers;

namespace Hedron.Definition
{
    public static class ISceneConfigUtil
    {
        public static Offset<SceneConfig> ConstructSceneConfig(FlatBufferBuilder builder, ISceneConfig feature)
        {
            var lightOffset = builder.CreateString(feature.LightConfig().GetName());

            SceneConfig.StartSceneConfig(builder);
            SceneConfig.AddGenerateProxyGeo(builder, feature.ShouldGenerateProxyGeo());
            SceneConfig.AddLightConfig(builder, lightOffset);
            return SceneConfig.EndSceneConfig(builder);
        }
    }

    [Definition(Version: 0, Name: "sceneconfig")]
    public abstract class ISceneConfig : IDefinition
    {
        public abstract bool ShouldGenerateProxyGeo();
        public abstract ILightConfig LightConfig();

        public override void RegisterDependencies()
        {
            base.RegisterDependencies();
            AddDependency(LightConfig());
        }

        public override byte[] SerializeData()
        {
            FlatBufferBuilder builder = new FlatBufferBuilder(1024);
            var offset = ISceneConfigUtil.ConstructSceneConfig(builder, this);
            builder.Finish(offset.Value);
            return builder.SizedByteArray();
        }
    }
}
