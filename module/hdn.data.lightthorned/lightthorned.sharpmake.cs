using Sharpmake;

[Generate]
public class DataLightthornedCoreProject : CSharpProject
{
    public DataLightthornedCoreProject()
    {
        Name = "data.lightthorned";
        SourceRootPath = @"[project.SharpmakeCsPath]/src";
        AddTargets(TargetUtil.DefaultCSharpTarget8_0);
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.DotNetClassLibrary;

        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<HdefProject>(target);
        // conf.EventPreBuild.Add("rd /s /q $(ProjectDir)generated");
        // conf.EventPreBuild.Add("call $(ProjectDir)generate_flatbuffers.bat");
    }
}