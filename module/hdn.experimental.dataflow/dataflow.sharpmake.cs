using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class DataFlowProject : BaseCppProject
{
    public DataFlowProject()
    {
        Name = "dataflow";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<StbImageProject>(target);
        conf.AddPublicDependency<XXHashProject>(target);
        conf.AddPublicDependency<ConfigProject>(target);
        conf.AddPublicDependency<CLI11Project>(target);
        conf.AddPublicDependency<FmtProject>(target);
    }
}