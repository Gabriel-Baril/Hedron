using System.IO;
using Sharpmake;

[Generate]
public class LightthornedSolution : Solution
{
    public LightthornedSolution()
    {
        Name = "lightthorned";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<LightthornedProject>(target);

        conf.SetStartupProject<LightthornedProject>();
    }
}