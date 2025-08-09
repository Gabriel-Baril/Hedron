using Sharpmake; // Contains the entire Sharpmake object library.
using System.IO; // For Path.Combine

[Generate]
public class FmodPlaygroundProject : BaseCppProject
{
    public FmodPlaygroundProject()
    {
        Name = "fmod-playground";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.EXPERIMENTAL_VS_CATEGORY;
        
        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        string fmodSDK = System.Environment.GetEnvironmentVariable(Constants.FMOD_SDK_ENV);
        if (string.IsNullOrEmpty(fmodSDK))
        {
            throw new System.Exception("FMOD SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(fmodSDK, "api\\core\\inc"));
        
        if (target.Platform == Platform.win64)
        {
            if (target.Optimization == Optimization.Debug)
            {
                string sourceLibraryPath = Path.Combine(fmodSDK, "api\\core\\lib\\x64\\");
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "fmodL_vc", "fmodL", false, true);
            }
            else if (target.Optimization == Optimization.Release || target.Optimization == Optimization.Retail)
            {
                string sourceLibraryPath = Path.Combine(fmodSDK, "api\\core\\lib\\x64\\");
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "fmod_vc", "fmod", false, true);
            }
        }
        else if (target.Platform == Platform.win32)
        {
            if (target.Optimization == Optimization.Debug)
            {
                string sourceLibraryPath = Path.Combine(fmodSDK, "api\\core\\lib\\x86\\");
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "fmodL_vc", "fmodL", true, true);
            }
            else if (target.Optimization == Optimization.Release || target.Optimization == Optimization.Retail)
            {
                string sourceLibraryPath = Path.Combine(fmodSDK, "api\\core\\lib\\x86\\");
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "fmod_vc", "fmod", false, true);
            }

        }

        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<HobjProject>(target);
        conf.AddPublicDependency<XXHashProject>(target);
    }
}