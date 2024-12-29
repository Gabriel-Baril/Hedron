using Sharpmake;

[Generate]
public class HdefSolution : CSharpSolution
{
    public HdefSolution()
    {
        Name = "hdef";
        AddTargets(TargetUtil.DefaultCSharpTarget8_0);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<HdefProject>(target);
        conf.AddProject<HdefcProject>(target);
        conf.AddProject<DataLightthornedCoreProject>(target);
        conf.AddProject<DataLightthornedPack0Project>(target);
    }
}