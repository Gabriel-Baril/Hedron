using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class SheredomJsonProject : Project
{
    public SheredomJsonProject()
    {
        Name = "sheredom-json";
        SourceRootPath = @"[project.SharpmakeCsPath]\Vendor";
        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release));
    }
    
    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.None;
        conf.IncludePaths.Add(SourceRootPath);
    }
}