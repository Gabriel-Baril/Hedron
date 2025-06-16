using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class PipelineProject : BaseCppProject
{
    public PipelineProject()
    {
        Name = "pipeline";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.TOOL_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<ConfigProject>(target);
        conf.AddPublicDependency<PugiXMLProject>(target);
        conf.AddPublicDependency<FlecsProject>(target);
        conf.AddPublicDependency<HobjProject>(target);
        conf.AddPublicDependency<CLI11Project>(target);

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
    }
}