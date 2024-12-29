using CommandLine;
using CommandLine.Text;
using System;
using System.Collections.Generic;
using System.Linq;

#nullable enable

namespace Hedron.Client
{
    using Hedron.Feature;
    using System.Reflection;

    class Program
    {
        public class Options
        {
            [Option('a', "assembly", Required = true, HelpText = "The assembly containing the feature descriptor")]
            public string AssemblyPath { get; set; } = string.Empty;


            [Option('f', "feature", Required = false, HelpText = "The feature to compile")]
            public string FeatureName { get; set; } = string.Empty;

            [Option('o', "out", Required = true, HelpText = "The output feature folder")]
            public string OutFolder { get; set; } = string.Empty;


            [Option('r', "recursive", Required = false, HelpText = "Should the dependencies of the feature also be compiled?")]
            public bool RecursiveFeatureCompilation { get; set; } = false;


            [Option('b', "binder", Required = true, HelpText = "Which binder parameter should we use?")]
            public string BinderName { get; set; } = string.Empty;

            [Option('a', "all", Required = false, HelpText = "Convert all features in the specified assembly?")]
            public bool CompileAll { get; set; } = false;

        }

        public static IEnumerable<Type> GetFeatureTypes(Assembly assembly)
        {
            return assembly.GetTypes().Where(type => type.GetCustomAttributes(typeof(FeatureAttribute), false).Length > 0);
        }

        static void Main(string[] args)
        {
            Parser.Default.ParseArguments<Options>(args)
                .WithParsed(opts => Run(opts))
                .WithNotParsed(errors => HandleErrors(errors));
        }

        public static Type? GetFeature(Assembly assembly, string featureName)
        {
            Type? featureType = assembly.GetTypes().FirstOrDefault(t => t.Name == featureName); ;

            if (featureType != null)
            {
                Console.WriteLine($"{featureName} feature found");
            }
            else
            {
                Console.WriteLine($"Cannot find {featureName} feature");
            }
            return featureType;
        }

        public static void CompileFeature(Type binderType, Type featureType, string outFolder, bool recursiveCompilation)
        {
            object? featureInstance = Activator.CreateInstance(featureType);

            MethodInfo? bindMethod = featureType.GetMethod("SetBinderFromType");
            if (bindMethod != null)
            {
                object[] methodArgs = new object[] { binderType };
                bindMethod.Invoke(featureInstance, methodArgs);
            }
            else
            {
                Console.WriteLine($"Cannot find bind method in feature!");
            }

            MethodInfo? saveMethod = recursiveCompilation ? featureType.GetMethod("SaveAll") : featureType.GetMethod("Save");
            if (saveMethod != null)
            {
                object[] methodArgs = new object[] { outFolder };
                saveMethod.Invoke(featureInstance, methodArgs);
            }
            else
            {
                Console.WriteLine($"Cannot find saving method in feature!");
            }
        }

        static void Run(Options opts)
        {
            Console.WriteLine($"RecursiveFeatureCompilation : {opts.RecursiveFeatureCompilation}");
            Console.WriteLine($"AssemblyPath : {opts.AssemblyPath}");
            Console.WriteLine($"BinderName : {opts.BinderName}");
            Console.WriteLine($"FeatureName : {opts.FeatureName}");
            Console.WriteLine($"OutFolder : {opts.OutFolder}");

            // Load Assembly Path
            Assembly assembly = Assembly.LoadFrom(opts.AssemblyPath);

            // Find Feature Binder Class
            Type? binderType = GetFeature(assembly, opts.BinderName);
            if (binderType == null)
            {
                Console.WriteLine("Binder feature not found cancelling...");
                return;
            }

            // Find feature class

            if (opts.FeatureName != string.Empty)
            {
                Type? featureType = GetFeature(assembly, opts.FeatureName);
                if (featureType == null)
                {
                    Console.WriteLine("Feature not found cancelling...");
                    return;
                }

                CompileFeature(binderType, featureType, opts.OutFolder, opts.RecursiveFeatureCompilation);
            }
            else if (opts.CompileAll)
            {
                foreach (var featureType in GetFeatureTypes(assembly))
                {
                    CompileFeature(binderType, featureType, opts.OutFolder, opts.RecursiveFeatureCompilation);
                }
            }
        }

        static void HandleErrors(IEnumerable<Error> errors)
        {
            var sentenceBuilder = SentenceBuilder.Create();
            foreach (Error error in errors)
            {
                Console.WriteLine(sentenceBuilder.FormatError(error));
            }
        }
    }
}