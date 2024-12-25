using Sharpmake;

[Generate]
public class IdaesSolution : Solution
{
    public IdaesSolution()
    {
        Name = "idaes";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<SheredomJsonProject>(target);
        conf.AddProject<IdaesProject>(target);

        conf.SetStartupProject<IdaesProject>();
    }
}