using Sharpmake; // Contains the entire Sharpmake object library.
using System.IO; // For Path.Combine

[Generate]
public class VulkanPlaygroundProject : BaseCppProject
{
    public VulkanPlaygroundProject()
    {
        Name = "vkplayground";
        SourceRootPath = @"[project.SharpmakeCsPath]\src";
        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public new void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        string vulkanSDK = System.Environment.GetEnvironmentVariable("VULKAN_SDK");
        if (string.IsNullOrEmpty(vulkanSDK))
        {
            throw new System.Exception("VULKAN_SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(vulkanSDK, "Include"));
        conf.LibraryPaths.Add(Path.Combine(vulkanSDK, "Lib"));
        conf.LibraryFiles.Add("vulkan-1.lib");

        conf.Output = Project.Configuration.OutputType.Exe;
        conf.TargetPath = @"[project.SharpmakeCsPath]\Out\Bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\Out\Intermediate\[target.Platform]-[target.Optimization]";

        conf.AddPublicDependency<TinyObjLoaderProject>(target);
        conf.AddPublicDependency<GLFWProject>(target);
        conf.AddPublicDependency<ImguiProject>(target);
        conf.AddPublicDependency<CoreProject>(target);

        // TODO: Ideon specific dependencies, remove once we have a proper way to have an independent ui setup for tools 
        conf.AddPublicDependency<PugiXMLProject>(target);
    }
}