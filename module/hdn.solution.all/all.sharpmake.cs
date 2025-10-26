using Sharpmake;

[Generate]
public class AllSolution : Solution
{
    public AllSolution()
    {
        Name = "all";

        AddTargets(TargetUtil.DefaultTarget);
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        // We don't really need to test the compilation of external projects, they should already be working properly.
        // They will also be indirectly tested by project depending on them
        // Visual Studio will also trigger an error for header only projects/module 

        // conf.AddProject<Catch2Project>(target);
        // conf.AddProject<CLI11Project>(target);
        // conf.AddProject<EABaseProject>(target);
        // conf.AddProject<EASTLProject>(target);
        // conf.AddProject<FlatbuffersProject>(target);
        // conf.AddProject<FmtProject>(target);
        // conf.AddProject<GLFWProject>(target);
        // conf.AddProject<GlmProject>(target);
        // conf.AddProject<ImguiProject>(target);
        // conf.AddProject<INIHProject>(target);
        // conf.AddProject<NlohmannJsonProject>(target);
        // conf.AddProject<OctreeCppProject>(target);
        // conf.AddProject<OpenFBXProject>(target);
        // conf.AddProject<PugiXMLProject>(target);
        // conf.AddProject<SheredomJsonProject>(target);
        // conf.AddProject<SpdlogProject>(target);
        // conf.AddProject<StbImageProject>(target);
        // conf.AddProject<TinyProcessLibraryProject>(target);
        // conf.AddProject<TracyProject>(target);
        // conf.AddProject<TinyObjLoaderProject>(target);
        // conf.AddProject<XXHashProject>(target);

        conf.AddProject<HdnCodeModuleCoreProject>(target);
        conf.AddProject<HdnTestModuleCore>(target);
        conf.AddProject<HdnCodeModuleHobjProject>(target);
        conf.AddProject<HdnCodePlaygroundAsyncProject>(target);
        conf.AddProject<HdnCodePlaygroundCfgProject>(target);
        conf.AddProject<HdnCodePlaygroundEcsProject>(target);
        conf.AddProject<HdnCodePlaygroundFmodProject>(target);
        conf.AddProject<HdnCodePlaygroundHsonProject>(target);
        conf.AddProject<HdnCodePlaygroundZoneProject>(target);
        conf.AddProject<HdnCodeToolPipelineProject>(target);
        conf.AddProject<HdnCodeGameDreamlikeProject>(target);
        conf.AddProject<HdnCodeToolEditorProject>(target);
    }
}