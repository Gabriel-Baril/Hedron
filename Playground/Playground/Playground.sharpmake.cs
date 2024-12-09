using System.IO;
using Sharpmake;

[Generate]
public class PlaygroundSolution : Solution
{
    public PlaygroundSolution()
    {
        Name = "Playground";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<DataTraceProject>(target);

        conf.SetStartupProject<DataTraceProject>();
    }
}