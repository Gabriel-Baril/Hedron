using Sharpmake;

[Generate]
public class PolySolution : Solution
{
    public PolySolution()
    {
        Name = "poly";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<PolyProject>(target);

        conf.SetStartupProject<PolyProject>();
    }
}