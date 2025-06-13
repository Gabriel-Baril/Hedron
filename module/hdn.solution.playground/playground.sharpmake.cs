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
        conf.AddProject<ArchivePlaygroundProject>(target);
        conf.AddProject<AsyncPlaygroundProject>(target);
        conf.AddProject<CfgPlaygroundProject>(target);
        conf.AddProject<EcsPlaygroundProject>(target);
        conf.AddProject<HsonPlaygroundProject>(target);
        conf.AddProject<PipelineProject>(target);

        conf.SetStartupProject<EcsPlaygroundProject>();
    }
}