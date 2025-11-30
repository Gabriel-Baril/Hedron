using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodeToolHBuildProject : BaseCppProject
{
    public HdnCodeToolHBuildProject()
    {
        Name = "dm.code.tool.hbuild";
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

        conf.AddPublicDependency<HdnCodeModuleCoreProject>(target);
        conf.AddPublicDependency<HdnCodeModuleSharedProject>(target);
        conf.AddPublicDependency<HdnCodeModulePipelineProject>(target);
        conf.AddPublicDependency<HdnCodeExternalPugiXMLProject>(target);
        conf.AddPublicDependency<HdnCodeExternalFlecsProject>(target);
        conf.AddPublicDependency<HdnCodeModuleHobjProject>(target);
        conf.AddPublicDependency<HdnCodeExternalCLI11Project>(target);

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
    }
}
