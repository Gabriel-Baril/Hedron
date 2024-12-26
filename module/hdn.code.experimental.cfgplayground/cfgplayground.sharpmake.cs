using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class CfgPlaygroundProject : BaseCppProject
{
    public CfgPlaygroundProject()
    {
        Name = "cfgplayground";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\Out\Bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\Out\Intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<ConfigProject>(target);
        conf.AddPublicDependency<CLI11Project>(target);
    }
}