using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodePlaygroundAsyncProject : BaseCppProject
{
    public HdnCodePlaygroundAsyncProject()
    {
        Name = "dm.code.playground.async";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.PLAYGROUND_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<HdnCodeModuleCoreProject>(target);
    }
}
