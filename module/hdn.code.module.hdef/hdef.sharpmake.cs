using Sharpmake;


[Generate]
public class HdefProject : CSharpProject
{
    public HdefProject()
    {
        Name = "hdef";
        SourceRootPath = @"[project.SharpmakeCsPath]/src";
        AddTargets(TargetUtil.DefaultCSharpTarget8_0);
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.DotNetClassLibrary;

        conf.SolutionFolder = Constants.DATA_VS_CATEGORY;

        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.ReferencesByNuGetPackage.Add(Constants.NUGET_PACKAGE_FLATBUFFER_NAME, Constants.NUGET_PACKAGE_FLATBUFFER_VERSION);

        // conf.EventPreBuild.Add("rd /s /q $(ProjectDir)generated");
        // conf.EventPreBuild.Add("call $(ProjectDir)generate_flatbuffers.bat");
    }
}