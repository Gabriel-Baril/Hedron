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
        conf.AddProject<BuildPackagingProject>(target);
        conf.AddProject<CfgPlaygroundProject>(target);
        conf.AddProject<DataFlowProject>(target);
        conf.AddProject<EcsPlaygroundProject>(target);
        conf.AddProject<HdefPlaygroundProject>(target);
        conf.AddProject<FileLoadingProject>(target);
        conf.AddProject<ListenDirectoryProject>(target);
        conf.AddProject<SelfModifyingCodeProject>(target);

        conf.SetStartupProject<HdefPlaygroundProject>();
    }
}