using System.IO;
using Sharpmake;

[Generate]
public class PolySolution : Solution
{
    public PolySolution()
    {
        Name = "poly";

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<PolyProject>(target);

        conf.SetStartupProject<PolyProject>();
    }
}