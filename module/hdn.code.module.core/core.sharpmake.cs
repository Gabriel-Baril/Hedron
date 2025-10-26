using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodeModuleCoreProject : BaseCppProject
{
    public HdnCodeModuleCoreProject()
    {
        Name = "hdn.code.module.core";
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

        conf.Defines.Add("_CRT_SECURE_NO_WARNINGS");

        conf.AddPublicDependency<HdnCodeExternalGlmProject>(target);
        conf.AddPublicDependency<HdnCodeExternalSpdlogProject>(target);
        conf.AddPublicDependency<HdnCodeExternalEASTLProject>(target);
        conf.AddPublicDependency<HdnCodeExternalXXHashProject>(target);
        conf.AddPublicDependency<HdnCodeExternalTLSFProject>(target);
        conf.AddPublicDependency<HdnCodeExternalINIHProject>(target);
    }
}