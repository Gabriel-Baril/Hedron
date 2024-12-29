using Sharpmake;

[Generate]
public class DataLightthornedPack0Project : CSharpProject
{
    public DataLightthornedPack0Project()
    {
        Name = "data.lightthorned.pack0";
        RootPath = @"[project.SharpmakeCsPath]/src";
        AddTargets(TargetUtil.DefaultCSharpTarget8_0);
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.DotNetClassLibrary;

        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<HdefProject>(target);
        conf.AddPublicDependency<DataLightthornedCoreProject>(target);
    }
}