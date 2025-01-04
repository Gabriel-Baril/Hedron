using System.IO;
using Sharpmake;

[Generate]
public class PlaygroundSolution : Solution
{
    public PlaygroundSolution()
    {
        Name = "playground";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<DataFlowProject>(target);
        conf.AddProject<DataTraceProject>(target);
        conf.AddProject<CfgPlaygroundProject>(target);
        conf.AddProject<BuildPackagingProject>(target);
        conf.AddProject<SelfModifyingCodeProject>(target);

        conf.SetStartupProject<CfgPlaygroundProject>();
    }
}