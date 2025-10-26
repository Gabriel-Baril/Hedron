using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class HdnCodeModuleGfxProject : BaseCppProject
{
    public HdnCodeModuleGfxProject()
    {
        Name = "hdn.code.module.gfx";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.SolutionFolder = Constants.MODULE_VS_CATEGORY;

        conf.Output = Project.Configuration.OutputType.Lib;
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");

        string vulkanSDK = System.Environment.GetEnvironmentVariable(Constants.VULKAN_SDK_ENV);
        if (string.IsNullOrEmpty(vulkanSDK))
        {
            throw new System.Exception("VULKAN SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(vulkanSDK, "Include"));
        conf.LibraryFiles.Add(Path.Combine(vulkanSDK, "Lib", "vulkan-1.lib"));

        conf.AddPublicDependency<HdnCodeModuleCoreProject>(target);
        conf.AddPublicDependency<HdnCodeExternalTinyObjLoaderProject>(target);
        conf.AddPublicDependency<HdnCodeExternalOpenFBXProject>(target);
        conf.AddPublicDependency<HdnCodeExternalGLFWProject>(target);
    }
}