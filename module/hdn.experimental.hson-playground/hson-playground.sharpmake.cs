using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HsonPlaygroundProject : BaseCppProject
{
    public HsonPlaygroundProject()
    {
        Name = "hson-playground";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.EXPERIMENTAL_VS_CATEGORY;
        
        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<HobjProject>(target);
        conf.AddPublicDependency<StbImageProject>(target);
        conf.AddPublicDependency<XXHashProject>(target);
    }
}