namespace Hedron.Definition
{
    [Definition(Version: 0, Name: "filedef")]
    public abstract class IFileDefinition : IDefinition
    {
        // TODO: public abstract IFilePathFeature FilePath(); // For example, feature_output_path. You could have a chain of IFilePathFeature

        public abstract string FilePath();

        public override byte[] SerializeData()
        {
            return null;
        }
    }
}
