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
        conf.ReferencesByNuGetPackage.Add(Constants.NUGET_PACKAGE_COMMAND_LINE_PARSER_NAME, Constants.NUGET_PACKAGE_COMMAND_LINE_PARSER_VERSION);
        conf.ReferencesByNuGetPackage.Add(Constants.NUGET_PACKAGE_FLATBUFFER_NAME, Constants.NUGET_PACKAGE_FLATBUFFER_VERSION);
    }
}