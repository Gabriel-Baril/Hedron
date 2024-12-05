using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class PlaygroundProject : BaseCppProject
{
    public PlaygroundProject()
        : base()
    {
        Name = "Playground";
        SourceRootPath = @"[project.SharpmakeCsPath]\Source";
        AddTargets(TargetUtil.DefaultTarget);
    }
    
    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\Out\Bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\Out\Intermediate\[target.Platform]-[target.Optimization]";
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\Source");

        conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);

        conf.AddPublicDependency<CoreProject>(target);
    }
}