using CommandLine;
using CommandLine.Text;
using System;
using System.Collections.Generic;
using System.Linq;

#nullable enable

namespace Hedron.Client
{
    using Hedron.Definition;
    using System.Reflection;

    class Program
    {
        public class Options
        {
            [Option('a', "assembly", Required = true, HelpText = "The assembly containing the definition descriptor")]
            public string AssemblyPath { get; set; } = string.Empty;


            [Option('f', "definition", Required = false, HelpText = "The definition to compile")]
            public string DefinitionName { get; set; } = string.Empty;

            [Option('o', "out", Required = true, HelpText = "The output definition folder")]
            public string OutFolder { get; set; } = string.Empty;


            [Option('r', "recursive", Required = false, HelpText = "Should the dependencies of the definition also be compiled?")]
            public bool RecursiveDefinitionCompilation { get; set; } = false;

            [Option('a', "all", Required = false, HelpText = "Convert all definitions in the specified assembly?")]
            public bool CompileAll { get; set; } = false;

        }

        public static IEnumerable<Type> GetDefinitionTypes(Assembly assembly)
        {
            return assembly.GetTypes().Where(type => type.GetCustomAttributes(typeof(DefinitionAttribute), false).Length > 0);
        }

        static void Main(string[] args)
        {
            Parser.Default.ParseArguments<Options>(args)
                .WithParsed(opts => Run(opts))
                .WithNotParsed(errors => HandleErrors(errors));
        }

        public static Type? GetDefinition(Assembly assembly, string definitionName)
        {
            Type? definitionType = assembly.GetTypes().FirstOrDefault(t => t.Name == definitionName); ;

            if (definitionType != null)
            {
                Console.WriteLine($"{definitionName} definition found");
            }
            else
            {
                Console.WriteLine($"Cannot find {definitionName} definition");
            }
            return definitionType;
        }

        public static void CompileDefinition(Type definitionType, string outFolder, bool recursiveCompilation)
        {
            object? definitionInstance = Activator.CreateInstance(definitionType);

            MethodInfo? saveMethod = recursiveCompilation ? definitionType.GetMethod("SaveAll") : definitionType.GetMethod("Save");
            if (saveMethod != null)
            {
                object[] methodArgs = new object[] { outFolder };
                saveMethod.Invoke(definitionInstance, methodArgs);
            }
            else
            {
                Console.WriteLine($"Cannot find saving method in definition!");
            }
        }

        static void Run(Options opts)
        {
            Console.WriteLine($"RecursiveDefinitionCompilation : {opts.RecursiveDefinitionCompilation}");
            Console.WriteLine($"AssemblyPath : {opts.AssemblyPath}");
            Console.WriteLine($"DefinitionName : {opts.DefinitionName}");
            Console.WriteLine($"OutFolder : {opts.OutFolder}");

            // Load Assembly Path
            Assembly assembly = Assembly.LoadFrom(opts.AssemblyPath);

            // Find definition class
            if (opts.DefinitionName != string.Empty)
            {
                Type? definitionType = GetDefinition(assembly, opts.DefinitionName);
                if (definitionType == null)
                {
                    Console.WriteLine("Definition not found cancelling...");
                    return;
                }

                CompileDefinition(definitionType, opts.OutFolder, opts.RecursiveDefinitionCompilation);
            }
            else if (opts.CompileAll)
            {
                foreach (var definitionType in GetDefinitionTypes(assembly))
                {
                    CompileDefinition(definitionType, opts.OutFolder, opts.RecursiveDefinitionCompilation);
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