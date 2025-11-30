using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodeModulePipelineProject : BaseCppProject
{
    public HdnCodeModulePipelineProject()
    {
        Name = "dm.code.module.pipeline";
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

        conf.Defines.Add("_CRT_SECURE_NO_WARNINGS");

        conf.AddPublicDependency<HdnCodeModuleCoreProject>(target);
        conf.AddPublicDependency<HdnCodeExternalPugiXMLProject>(target);
        conf.AddPublicDependency<HdnCodeExternalFlecsProject>(target);
        conf.AddPublicDependency<HdnCodeExternalFlatbuffersProject>(target);
        conf.AddPublicDependency<HdnCodeExternalTLSFProject>(target);

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
    }
}
