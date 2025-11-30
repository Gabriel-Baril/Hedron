using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodeModuleHobjProject : BaseCppProject
{
    public HdnCodeModuleHobjProject()
    {
        Name = "dm.code.module.hobj";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.MODULE_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Lib;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");

        conf.AddPublicDependency<HdnCodeModuleCoreProject>(target);
        conf.AddPublicDependency<HdnCodeExternalFlecsProject>(target);
        conf.AddPublicDependency<HdnCodeExternalPugiXMLProject>(target);
    }
}
