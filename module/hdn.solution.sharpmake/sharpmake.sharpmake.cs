using Sharpmake;
using System.Collections.Generic;
using System.IO;

[Generate]
public class SharpmakeFilesProject : CSharpProject
{
    public SharpmakeFilesProject()
    {
        Name = "sharpmake";
        RootPath = @"[project.SharpmakeCsPath]";
        SourceFilesExtensions.Clear();

        AddTargets(TargetUtil.DefaultCSharpTarget6_0);

        foreach (var folder in Constants.SHARPMAKE_FOLDERS)
        {
            string fullPath = Path.Combine(Directory.GetCurrentDirectory(), folder);
            if (Directory.Exists(fullPath))
            {
                List<string> files = new List<string>();

                files.AddRange(Directory.GetFiles(
                    fullPath,
                    "*.sharpmake.cs",
                    SearchOption.AllDirectories // Recursive search within each folder
                ));

                files.AddRange(Directory.GetFiles(
                    fullPath,
                    "*.vcxproj",
                    SearchOption.AllDirectories // Recursive search within each folder
                ));

                files.AddRange(Directory.GetFiles(
                    fullPath,
                    "*.csproj",
                    SearchOption.AllDirectories // Recursive search within each folder
                ));

                foreach (string file in files)
                {
                    SourceFiles.Add(file);
                }
            }
        }
        SourceFiles.Add(Path.Combine(Paths.MainFileDirectory, Constants.MAIN_SHARPMAKE_FILE));
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.ReferencesByPath.Add(Path.Combine(Paths.MainFileDirectory, @"Vendor\Sharpmake\Sharpmake.Application\bin\Release\net6.0\Sharpmake.dll"));
        conf.TargetPath = @"[project.SharpmakeCsPath]\out\bin\[target.Platform]-[target.Optimization]";
        conf.IntermediatePath = @"[project.SharpmakeCsPath]\out\intermediate\[target.Platform]-[target.Optimization]";
    }
}

[Generate]
public class AllSharpmakeSolution : CSharpSolution
{
    public AllSharpmakeSolution()
    {
        Name = "sharpmake";
        AddTargets(TargetUtil.DefaultCSharpTarget6_0);

    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.AddProject<SharpmakeFilesProject>(target);

        conf.SetStartupProject<SharpmakeFilesProject>();
    }
}