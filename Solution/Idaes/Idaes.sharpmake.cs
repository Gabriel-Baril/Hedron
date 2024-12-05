using System.IO;
using Sharpmake;

[Generate]
public class IdaesSolution : Solution
{
    public IdaesSolution()
    {
        Name = "Idaes";

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<SheredomJsonProject>(target);
        conf.AddProject<IdaesProject>(target);
    }
}