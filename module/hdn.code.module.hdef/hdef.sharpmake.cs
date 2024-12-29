using Sharpmake;

[Generate]
public class HdefProject : CSharpProject
{
    public HdefProject()
    {
        Name = "hdef";
        RootPath = @"[project.SharpmakeCsPath]/src";
        AddTargets(TargetUtil.DefaultCSharpTarget8_0);
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.DotNetClassLibrary;

        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.ReferencesByNuGetPackage.Add("Google.FlatBuffers", "24.3.25");
        conf.EventPreBuild.Add("rd /s /q $(ProjectDir)generated");
        conf.EventPreBuild.Add("call $(ProjectDir)generate_flatbuffers.bat");
    }
}