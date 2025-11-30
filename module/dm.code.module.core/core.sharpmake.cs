using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class DmCodeModuleCoreProject : BaseCppProject
{
    public DmCodeModuleCoreProject()
    {
        Name = "dm.code.module.core";
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

        conf.AddPublicDependency<DmCodeExternalGlmProject>(target);
        conf.AddPublicDependency<DmCodeExternalSpdlogProject>(target);
        conf.AddPublicDependency<DmCodeExternalEASTLProject>(target);
        conf.AddPublicDependency<DmCodeExternalXXHashProject>(target);
        conf.AddPublicDependency<DmCodeExternalTLSFProject>(target);
        conf.AddPublicDependency<DmCodeExternalINIHProject>(target);
    }
}
