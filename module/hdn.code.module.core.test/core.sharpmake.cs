using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class CoreProjectTest : BaseCppTestProject
{
    public CoreProjectTest()
    {
        Name = "core.test";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.TEST_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\Out\Bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\Out\Intermediate\[target.Platform]-[target.Optimization]";
        // conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\include");
        // conf.Defines.Add("_CRT_SECURE_NO_WARNINGS");

        conf.AddPublicDependency<Catch2Project>(target);
        conf.AddPublicDependency<CoreProject>(target);
    }
}