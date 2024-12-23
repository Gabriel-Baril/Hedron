using Sharpmake;

[Generate]
public class IdeonSolution : Solution
{
    public IdeonSolution()
    {
        Name = "ideon";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<IdeonProject>(target);

        conf.SetStartupProject<IdeonProject>();
    }
}