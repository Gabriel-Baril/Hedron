using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class DmCodePlaygroundEcsProject : BaseCppProject
{
    public DmCodePlaygroundEcsProject()
    {
        Name = "dm.code.playground.ecs";
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

        conf.AddPublicDependency<DmCodeModuleCoreProject>(target);
        conf.AddPublicDependency<DmCodeExternalFlecsProject>(target);
        conf.AddPublicDependency<DmCodeExternalNlohmannJsonProject>(target);
    }
}
