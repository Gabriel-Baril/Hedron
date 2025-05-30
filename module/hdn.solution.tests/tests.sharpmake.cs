using Sharpmake;

[Generate]
public class TestsSolution : Solution
{
    public TestsSolution()
    {
        Name = "tests";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<Catch2Project>(target); // The test framework used for c++
        conf.AddProject<CoreProjectTest>(target);

        conf.SetStartupProject<Catch2Project>();
    }
}