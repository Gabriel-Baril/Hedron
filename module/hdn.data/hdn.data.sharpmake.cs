using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class DataProject : BaseCppProject
{
    public DataProject()
    {
        Name = "hdn.data";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.DATA_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Lib;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");

        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<FlatbuffersProject>(target);

        // TODO: Add EventPreBuild which run build_data_projects.bat
        conf.EventPreBuild.Add($"call \"build_data_projects.bat\"");
    }
}