using Sharpmake;
using System;
using System.Diagnostics;
using System.IO;

[Generate]
public class SharpmakeFilesProject : CSharpProject
{
    public SharpmakeFilesProject()
    {
        Name = "sharpmake";
        RootPath = @"[project.SharpmakeCsPath]";
        SourceFilesExtensions.Clear();

        AddTargets(new Target(
                Platform.anycpu,
                DevEnv.vs2022,
                Optimization.Debug | Optimization.Release,
                OutputType.Dll,
                Blob.NoBlob,
                BuildSystem.MSBuild,
                DotNetFramework.net6_0));

        foreach (var folder in Constants.SHARPMAKE_FOLDERS)
        {
            string fullPath = Path.Combine(Directory.GetCurrentDirectory(), folder);
            if (Directory.Exists(fullPath))
            {
                string[] sharpmakeFiles = Directory.GetFiles(
                    fullPath,
                    "*.sharpmake.cs",
                    SearchOption.AllDirectories // Recursive search within each folder
                );

                foreach (string file in sharpmakeFiles)
                {
                    SourceFiles.Add(file);
                }
            }
        }

        SourceFiles.Add(@"[project.SharpmakeCsPath]\AllSharpmake.sharpmake.cs");
        SourceFiles.Add(Path.Combine(Paths.MainFileDirectory, Constants.MAIN_SHARPMAKE_FILE));
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.ReferencesByPath.Add(Path.Combine(Paths.MainFileDirectory, @"Vendor\Sharpmake\Sharpmake.Application\bin\Release\net6.0\Sharpmake.dll"));
    }
}

[Generate]
public class AllSharpmakeSolution : CSharpSolution
{
    public AllSharpmakeSolution()
    {
        Name = "sharpmake";
        AddTargets(new Target(
                Platform.anycpu,
                DevEnv.vs2022,
                Optimization.Debug | Optimization.Release,
                OutputType.Dll,
                Blob.NoBlob,
                BuildSystem.MSBuild,
                DotNetFramework.net6_0));

    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<SharpmakeFilesProject>(target);

        conf.SetStartupProject<SharpmakeFilesProject>();
    }
}