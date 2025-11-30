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

        // conf.AddProject<DmCodeExternalCatch2Project>(target);
        // conf.AddProject<DmCodeExternalCLI11Project>(target);
        // conf.AddProject<DmCodeExternalEABaseProject>(target);
        // conf.AddProject<DmCodeExternalEASTLProject>(target);
        // conf.AddProject<DmCodeExternalFlatbuffersProject>(target);
        // conf.AddProject<DmCodeExternalFmtProject>(target);
        // conf.AddProject<DmCodeExternalGLFWProject>(target);
        // conf.AddProject<DmCodeExternalGlmProject>(target);
        // conf.AddProject<DmCodeExternalImguiProject>(target);
        // conf.AddProject<DmCodeExternalINIHProject>(target);
        // conf.AddProject<DmCodeExternalNlohmannJsonProject>(target);
        // conf.AddProject<DmCodeExternalOctreeCppProject>(target);
        // conf.AddProject<DmCodeExternalOpenFBXProject>(target);
        // conf.AddProject<DmCodeExternalPugiXMLProject>(target);
        // conf.AddProject<DmCodeExternalSheredomJsonProject>(target);
        // conf.AddProject<DmCodeExternalSpdlogProject>(target);
        // conf.AddProject<DmCodeExternalStbImageProject>(target);
        // conf.AddProject<DmCodeExternalTinyProcessLibraryProject>(target);
        // conf.AddProject<DmCodeExternalTracyProject>(target);
        // conf.AddProject<DmCodeExternalTinyObjLoaderProject>(target);
        // conf.AddProject<DmCodeExternalXXHashProject>(target);

        conf.AddProject<DmCodeModuleCoreProject>(target);
        conf.AddProject<DmTestModuleCore>(target);
        conf.AddProject<DmCodeModuleHobjProject>(target);
        conf.AddProject<DmCodeModulePipelineProject>(target);
        conf.AddProject<DmCodePlaygroundAsyncProject>(target);
        conf.AddProject<DmCodePlaygroundCfgProject>(target);
        conf.AddProject<DmCodePlaygroundEcsProject>(target);
        conf.AddProject<DmCodePlaygroundFmodProject>(target);
        conf.AddProject<DmCodePlaygroundHsonProject>(target);
        conf.AddProject<DmCodePlaygroundZoneProject>(target);
        conf.AddProject<DmCodeToolHBuildProject>(target);
        conf.AddProject<DmCodeGameDreamlikeProject>(target);
        conf.AddProject<DmCodeToolEditorProject>(target);
    }
}
