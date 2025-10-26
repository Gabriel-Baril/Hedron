using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodePlaygroundHsonProject : BaseCppProject
{
    public HdnCodePlaygroundHsonProject()
    {
        Name = "hdn.code.playground.hson";
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
        conf.AddPublicDependency<HdnCodeModuleHobjProject>(target);
        conf.AddPublicDependency<StbImageProject>(target);
        conf.AddPublicDependency<XXHashProject>(target);
    }
}