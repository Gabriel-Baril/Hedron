using Sharpmake;

[Generate]
public class ToolsSolution : Solution
{
    public ToolsSolution()
    {
        Name = "tools";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<IdeonProject>(target);
        conf.AddProject<IdaesProject>(target);
        conf.AddProject<PolyProject>(target);
        conf.AddProject<ArchiveProject>(target);

        conf.SetStartupProject<ArchiveProject>();
    }
}