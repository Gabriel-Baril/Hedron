using System.IO; // For Path.Combine
using Sharpmake; // Contains the entire Sharpmake object library.

[Generate]
public class ArchiveProject : BaseCppProject
{
    public ArchiveProject()
    {
        Name = "archive";
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
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\src");
        
        string llvmSDK = System.Environment.GetEnvironmentVariable("LLVM_SDK");
        if (string.IsNullOrEmpty(llvmSDK))
        {
            throw new System.Exception("LLVM_SDK not found!");
        }
        conf.IncludePaths.Add(Path.Combine(llvmSDK, "include"));
        conf.LibraryPaths.Add(Path.Combine(llvmSDK, "lib"));
        conf.LibraryFiles.Add("libclang.lib");
        conf.LibraryFiles.Add("clangTooling.lib");
        conf.LibraryFiles.Add("clangBasic.lib");
        conf.LibraryFiles.Add("clangAST.lib");
        conf.LibraryFiles.Add("clangASTMatchers.lib");
        conf.LibraryFiles.Add("clangFrontend.lib");
        conf.LibraryFiles.Add("clangLex.lib");
        conf.LibraryFiles.Add("clangParse.lib");
        conf.LibraryFiles.Add("clangSema.lib");
        conf.LibraryFiles.Add("clangDriver.lib");
        conf.LibraryFiles.Add("clangEdit.lib");
        conf.LibraryFiles.Add("clangRewrite.lib");
        conf.LibraryFiles.Add("clangSerialization.lib");
        conf.LibraryFiles.Add("LLVMCore.lib");
        conf.LibraryFiles.Add("LLVMSupport.lib");


        conf.AddPublicDependency<GlmProject>(target);
        conf.AddPublicDependency<SpdlogProject>(target);
        conf.AddPublicDependency<CoreProject>(target);
        conf.AddPublicDependency<ArchiveLibProject>(target);
    }
}