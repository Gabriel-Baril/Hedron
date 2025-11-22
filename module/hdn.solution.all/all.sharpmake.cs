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

        // conf.AddProject<HdnCodeExternalCatch2Project>(target);
        // conf.AddProject<HdnCodeExternalCLI11Project>(target);
        // conf.AddProject<HdnCodeExternalEABaseProject>(target);
        // conf.AddProject<HdnCodeExternalEASTLProject>(target);
        // conf.AddProject<HdnCodeExternalFlatbuffersProject>(target);
        // conf.AddProject<HdnCodeExternalFmtProject>(target);
        // conf.AddProject<HdnCodeExternalGLFWProject>(target);
        // conf.AddProject<HdnCodeExternalGlmProject>(target);
        // conf.AddProject<HdnCodeExternalImguiProject>(target);
        // conf.AddProject<HdnCodeExternalINIHProject>(target);
        // conf.AddProject<HdnCodeExternalNlohmannJsonProject>(target);
        // conf.AddProject<HdnCodeExternalOctreeCppProject>(target);
        // conf.AddProject<HdnCodeExternalOpenFBXProject>(target);
        // conf.AddProject<HdnCodeExternalPugiXMLProject>(target);
        // conf.AddProject<HdnCodeExternalSheredomJsonProject>(target);
        // conf.AddProject<HdnCodeExternalSpdlogProject>(target);
        // conf.AddProject<HdnCodeExternalStbImageProject>(target);
        // conf.AddProject<HdnCodeExternalTinyProcessLibraryProject>(target);
        // conf.AddProject<HdnCodeExternalTracyProject>(target);
        // conf.AddProject<HdnCodeExternalTinyObjLoaderProject>(target);
        // conf.AddProject<HdnCodeExternalXXHashProject>(target);

        conf.AddProject<HdnCodeModuleCoreProject>(target);
        conf.AddProject<HdnTestModuleCore>(target);
        conf.AddProject<HdnCodeModuleHobjProject>(target);
        conf.AddProject<HdnCodeModulePipelineProject>(target);
        conf.AddProject<HdnCodePlaygroundAsyncProject>(target);
        conf.AddProject<HdnCodePlaygroundCfgProject>(target);
        conf.AddProject<HdnCodePlaygroundEcsProject>(target);
        conf.AddProject<HdnCodePlaygroundFmodProject>(target);
        conf.AddProject<HdnCodePlaygroundHsonProject>(target);
        conf.AddProject<HdnCodePlaygroundZoneProject>(target);
        conf.AddProject<HdnCodeToolHBuildProject>(target);
        conf.AddProject<HdnCodeGameDreamlikeProject>(target);
        conf.AddProject<HdnCodeToolEditorProject>(target);
    }
}
