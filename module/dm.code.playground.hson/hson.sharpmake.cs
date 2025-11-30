using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class DmCodePlaygroundHsonProject : BaseCppProject
{
    public DmCodePlaygroundHsonProject()
    {
        Name = "dm.code.playground.hson";
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
        conf.AddPublicDependency<DmCodeModuleHobjProject>(target);
        conf.AddPublicDependency<DmCodeExternalStbImageProject>(target);
        conf.AddPublicDependency<DmCodeExternalXXHashProject>(target);
    }
}
