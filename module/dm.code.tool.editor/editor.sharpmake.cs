using Sharpmake; // Contains the entire Sharpmake object library.
using System.IO; // For Path.Combine

[Generate]
public class DmCodeToolEditorProject : BaseCppProject
{
    public DmCodeToolEditorProject()
    {
        Name = "dm.code.tool.editor";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.TOOL_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";

        string physxSDK = System.Environment.GetEnvironmentVariable(Constants.PHYSX_SDK_ENV);
        if (string.IsNullOrEmpty(physxSDK))
        {
            throw new System.Exception("PHYSX SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(physxSDK, "include"));

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

        conf.AddPublicDependency<DmCodeModuleCoreProject>(target);
        conf.AddPublicDependency<DmCodeExternalTinyObjLoaderProject>(target);
        conf.AddPublicDependency<DmCodeExternalOpenFBXProject>(target);
        conf.AddPublicDependency<DmCodeExternalGLFWProject>(target);
        conf.AddPublicDependency<DmCodeExternalImguiProject>(target);
        conf.AddPublicDependency<DmCodeExternalFlecsProject>(target);
        conf.AddPublicDependency<DmCodeModuleGfxProject>(target);

        conf.AddPublicDependency<DmCodeExternalPugiXMLProject>(target);
        conf.AddPublicDependency<DmCodeExternalTinyProcessLibraryProject>(target);
        conf.AddPublicDependency<DmCodeExternalINIHProject>(target);
        conf.AddPublicDependency<DmCodeExternalCLI11Project>(target);
        conf.AddPublicDependency<DmCodeExternalFmtProject>(target);
        conf.AddPublicDependency<DmCodeExternalNlohmannJsonProject>(target);
    }
}
