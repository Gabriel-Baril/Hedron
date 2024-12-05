using System.IO;
using Sharpmake;

public static class Constants
{
    public const string MAIN_SHARPMAKE_FILE = "Main.sharpmake.cs";

    public const string EXTERNAL_FOLDER = "External";
    public const string MODULE_FOLDER = "Module";
    public const string PLUGIN_FOLDER = "Plugin";
    public const string SOLUTION_FOLDER = "Solution";
    public static string[] SHARPMAKE_FOLDERS = { Constants.EXTERNAL_FOLDER, Constants.MODULE_FOLDER, Constants.PLUGIN_FOLDER, Constants.SOLUTION_FOLDER };
}

public static class Paths
{
    public static readonly string MainFileDirectory = Directory.GetCurrentDirectory(); // Points to the home of the Main.sharpmake.cs
}

public static class Main
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments args)
    {
        args.Generate<IdaesSolution>();
        args.Generate<PlaygroundSolution>();
        
        args.Generate<AllSharpmakeSolution>();
    }
}
