using System.IO;
using Sharpmake;

[Generate]
public class IdeonSolution : Solution
{
    public IdeonSolution()
    {
        Name = "Ideon";

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<IdeonProject>(target);

        conf.SetStartupProject<IdeonProject>();
    }
}