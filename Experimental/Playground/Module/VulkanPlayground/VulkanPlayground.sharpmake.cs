using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class VulkanPlaygroundProject : BaseCppProject
{
    public VulkanPlaygroundProject()
    {
        Name = "VulkanPlayground";
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
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\Source\Public");

        conf.AddPublicDependency<GLFWProject>(target);
        conf.AddPublicDependency<CoreProject>(target);
    }
}