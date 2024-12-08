using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class SheredomJsonProject : Project
{
    public SheredomJsonProject()
    {
        Name = "sheredom-json";
        SourceRootPath = @"[project.SharpmakeCsPath]\Vendor";
        AddTargets(TargetUtil.DefaultTarget);
    }
    
    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.Output = Project.Configuration.OutputType.None;

        conf.SolutionFolder = Constants.EXTERNAL_FOLDER;

        conf.IncludePaths.Add(SourceRootPath);
    }
}