using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class DmCodeToolHBuildProject : BaseCppProject
{
    public DmCodeToolHBuildProject()
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

        conf.AddPublicDependency<DmCodeModuleCoreProject>(target);
        conf.AddPublicDependency<DmCodeModuleSharedProject>(target);
        conf.AddPublicDependency<DmCodeModulePipelineProject>(target);
        conf.AddPublicDependency<DmCodeExternalPugiXMLProject>(target);
        conf.AddPublicDependency<DmCodeExternalFlecsProject>(target);
        conf.AddPublicDependency<DmCodeModuleHobjProject>(target);
        conf.AddPublicDependency<DmCodeExternalCLI11Project>(target);

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
    }
}
