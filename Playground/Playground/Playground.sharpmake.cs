using System.IO;
using Sharpmake;

[Generate]
public class PlaygroundSolution : Solution
{
    public PlaygroundSolution()
    {
        Name = "Playground";

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<PlaygroundProject>(target);
    }
}