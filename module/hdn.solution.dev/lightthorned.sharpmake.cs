using System.IO;
using Sharpmake;

[Generate]
public class DevSolution : Solution
{
    public DevSolution()
    {
        Name = "dev";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<LightthornedProject>(target);
        conf.AddProject<EditorProject>(target);
        conf.AddProject<ArchiveProject>(target);

        conf.SetStartupProject<LightthornedProject>();
    }
}