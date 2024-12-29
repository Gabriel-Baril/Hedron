using Sharpmake;

[Generate]
public class HdefcProject : CSharpProject
{
    public HdefcProject()
    {
        Name = "hdefc";
        SourceRootPath = @"[project.SharpmakeCsPath]/src";
        AddTargets(TargetUtil.DefaultCSharpTarget8_0);
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.DotNetConsoleApp;

        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<HdefProject>(target);
        conf.ReferencesByNuGetPackage.Add("CommandLineParser", "2.9.1");
        conf.ReferencesByNuGetPackage.Add("Google.FlatBuffers", "24.3.25");
    }
}