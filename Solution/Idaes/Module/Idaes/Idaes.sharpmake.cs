using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class IdaesProject : Project
{
    public IdaesProject()
    {
        Name = "Idaes";
        SourceRootPath = @"[project.SharpmakeCsPath]\Source";
        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release));
    }
    
    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\Out\Bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\Out\Intermediate\[target.Platform]-[target.Optimization]";
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\Source");

        conf.AddPublicDependency<SheredomJsonProject>(target);
    }
}