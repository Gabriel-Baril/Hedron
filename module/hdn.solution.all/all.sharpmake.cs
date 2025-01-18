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
        // conf.AddProject<TinyObjLoaderProject>(target);
        // conf.AddProject<XXHashProject>(target);

        conf.AddProject<AsyncProject>(target);
        conf.AddProject<ConfigProject>(target);
        conf.AddProject<CoreProject>(target);
        conf.AddProject<CoreProjectTest>(target);
        conf.AddProject<AsyncPlaygroundProject>(target);
        conf.AddProject<BuildPackagingProject>(target);
        conf.AddProject<CfgPlaygroundProject>(target);
        conf.AddProject<DataFlowProject>(target);
        conf.AddProject<ListenDirectoryProject>(target);
        conf.AddProject<SelfModifyingCodeProject>(target);
        conf.AddProject<LightthornedProject>(target);
        conf.AddProject<PolyProject>(target);
        conf.AddProject<IdeonProject>(target);
        conf.AddProject<IdaesProject>(target);
    }
}