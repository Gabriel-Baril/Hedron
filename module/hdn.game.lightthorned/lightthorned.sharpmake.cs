using Sharpmake; // Contains the entire Sharpmake object library.
using System.IO; // For Path.Combine

[Generate]
public class LightthornedProject : BaseCppProject
{
    public LightthornedProject()
    {
        Name = "lightthorned";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.GAME_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        string vulkanSDK = System.Environment.GetEnvironmentVariable(Constants.VULKAN_SDK_ENV);
        if (string.IsNullOrEmpty(vulkanSDK))
        {
            throw new System.Exception("VULKAN SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(vulkanSDK, "Include"));
        conf.LibraryPaths.Add(Path.Combine(vulkanSDK, "Lib"));
        conf.LibraryFiles.Add("vulkan-1.lib");

        string physxSDK = System.Environment.GetEnvironmentVariable(Constants.PHYSX_SDK_ENV);
        if (string.IsNullOrEmpty(physxSDK))
        {
            throw new System.Exception("PHYSX SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(physxSDK, "include"));
        conf.LibraryPaths.Add(Path.Combine(physxSDK, "bin"));

        if (target.Platform == Platform.win32 || target.Platform == Platform.win64)
        {
            if (target.Optimization == Optimization.Debug)
            {
                string sourceLibraryPath = Path.Combine(physxSDK, "bin\\win.x86_64.vc143.mt\\debug\\");
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysX_64", true, true);
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysXFoundation_64", true, true);
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysXExtensions_static_64", true, false);
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysXCommon_64", true, true);
            }
            else if (target.Optimization == Optimization.Release || target.Optimization == Optimization.Retail)
            {
                string sourceLibraryPath = Path.Combine(physxSDK, "bin\\win.x86_64.vc143.mt\\release\\");
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysX_64", false, true);
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysXFoundation_64", false, true);
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysXExtensions_static_64", false, false);
                AddLib(conf, sourceLibraryPath, conf.TargetPath, "PhysXCommon_64", false, true);
            }
        }

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
        
        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<TinyObjLoaderProject>(target);
        conf.AddPublicDependency<OpenFBXProject>(target);
        conf.AddPublicDependency<GLFWProject>(target);
        conf.AddPublicDependency<ImguiProject>(target);
        conf.AddPublicDependency<FlecsProject>(target);
    }
}