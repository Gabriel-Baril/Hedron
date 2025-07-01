using Sharpmake; // Contains the entire Sharpmake object library.
using System.IO; // For Path.Combine

[Generate]
public class DummyProject : BaseCppProject
{
    public DummyProject()
    {
        Name = "dummy";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.GAME_VS_CATEGORY;
        
        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
        
        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<HobjProject>(target);
        conf.AddPublicDependency<GfxProject>(target);
    }
}