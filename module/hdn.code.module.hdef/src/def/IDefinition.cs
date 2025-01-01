using Google.FlatBuffers;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

#nullable enable

namespace Hedron.Definition
{
    public static class IDefinitionUtil
    {
        public delegate VectorOffset CreateDependencyVectorFunc(FlatBufferBuilder builder, Offset<Hedron.Definition.DefinitionSignature>[] list);

        public static Offset<DefinitionMetadata> ConstructDefinitionMetadata(FlatBufferBuilder builder, IDefinition definition)
        {
            var archTypeOffset = builder.CreateString(definition.GetType().BaseType?.Name);
            var typeChainOffset = builder.CreateString(TypeUtil.GetTypeChain(definition));
            DefinitionMetadata.StartDefinitionMetadata(builder);

            ModuleAttribute? attribute = (ModuleAttribute?)definition.GetType().GetCustomAttribute(typeof(ModuleAttribute), false);
            if (attribute != null)
            {
                DefinitionMetadata.AddVersion(builder, attribute.Version);
            }
            else
            {
                DefinitionMetadata.AddVersion(builder, 0);
            }

            DefinitionMetadata.AddArchType(builder, archTypeOffset);
            DefinitionMetadata.AddTypeChain(builder, typeChainOffset);
            return DefinitionMetadata.EndDefinitionMetadata(builder);
        }

        public static Offset<DefinitionSignature> ConstructDefinitionSignature(FlatBufferBuilder builder, IDefinition? definition)
        {
            string definitionName = string.Empty;
            if (definition != null)
            {
                definitionName = definition.GetName();
            }

            var nameOffset = builder.CreateString(definitionName);
            DefinitionSignature.StartDefinitionSignature(builder);
            DefinitionSignature.AddName(builder, nameOffset);
            return DefinitionSignature.EndDefinitionSignature(builder);
        }

        public static VectorOffset ConstructDefinitionSignatureList(FlatBufferBuilder builder, List<IDefinition> definitionList, CreateDependencyVectorFunc createDependencyVectorFunc)
        {
            var dependenciesOffsets = new List<Offset<DefinitionSignature>>();
            foreach (IDefinition definition in definitionList)
            {
                var offset = ConstructDefinitionSignature(builder, definition);
                dependenciesOffsets.Add(offset);
            }
            return createDependencyVectorFunc(builder, dependenciesOffsets.ToArray());
        }
    }

    public abstract class IDefinition
    {
        public bool Constructed { get; set; } = false;
        public List<IDefinition> Dependencies { get; set; } = new List<IDefinition>();
        IParameterBinder? Binder { get; set; } = null;

        public IDefinition()
        {
        }

        // Used by the definition compiler
        public void SetBinderFromType(Type binderType)
        {
            Binder = (IParameterBinder?)Activator.CreateInstance(binderType);
        }

        public dynamic GetParam(string ParamName, dynamic Default)
        {
            if (Binder == null)
            {
                return Default;
            }
            Binder.Construct();
            return Binder.Get(ParamName, Default);
        }

        public void SaveAll(string path)
        {
            Save(path);
            foreach (IDefinition definition in Dependencies)
            {
                definition.Binder = (definition is IParameterBinder) ? null : Binder;
                definition.SaveAll(path);
            }
        }

        public void Save(string outFolder)
        {
            Construct();
            string definitionName = GetName();

            List<DefinitionValidationMessage> validationMessages = Validate();
            foreach (DefinitionValidationMessage validationMessage in validationMessages)
            {
                switch (validationMessage.MessageType)
                {
                    case DefinitionValidationMessageType.Error:
                        Console.ForegroundColor = ConsoleColor.Red;
                        break;
                    case DefinitionValidationMessageType.Warning:
                        Console.ForegroundColor = ConsoleColor.Yellow;
                        break;
                    case DefinitionValidationMessageType.Info:
                        Console.ForegroundColor = ConsoleColor.Cyan;
                        break;
                }
                Console.WriteLine($"[{validationMessage.MessageType}][{definitionName}]: {validationMessage}");
                Console.ResetColor();
            }
            string outFile = $"{Path.Combine(outFolder, definitionName)}.hdef";
            Console.WriteLine($"Writing to {outFile}");
            File.WriteAllBytes(outFile, Serialize());
            // Print();
        }

        public byte[] Serialize()
        {
            FlatBufferBuilder builder = new FlatBufferBuilder(1024);
            var metadataOffset = IDefinitionUtil.ConstructDefinitionMetadata(builder, this);
            var signatureOffset = IDefinitionUtil.ConstructDefinitionSignature(builder, this);
            var binderOffset = IDefinitionUtil.ConstructDefinitionSignature(builder, Binder);
            var dependenciesOffset = IDefinitionUtil.ConstructDefinitionSignatureList(builder, Dependencies, Definition.CreateDependenciesVector);
            var dataOffset = Definition.CreateDataVector(builder, SerializeData());
            Definition.StartDefinition(builder);
            Definition.AddMetadata(builder, metadataOffset);
            Definition.AddSignature(builder, signatureOffset);
            Definition.AddBinder(builder, binderOffset);
            Definition.AddDependencies(builder, dependenciesOffset);
            Definition.AddData(builder, dataOffset);
            var definitionOffset = Definition.EndDefinition(builder);
            builder.Finish(definitionOffset.Value);
            return builder.SizedByteArray();
        }

        public virtual List<DefinitionValidationMessage> Validate()
        {
            return new List<DefinitionValidationMessage>();
        }

        public virtual void Construct()
        {
            if (Constructed)
            {
                return;
            }

            Constructed = true;
            RegisterDependencies();
        }

        public virtual void RegisterDependencies()
        {
            if (Binder != null)
            {
                // AddDependency(Binder);
            }
        }

        public abstract byte[] SerializeData();

        public string GetName()
        {
            return GetType().Name;
        }

        public virtual void Print()
        {
            Console.WriteLine($"Name: {GetName()}");
            if (Binder != null)
            {
                Console.WriteLine($"Binder: {Binder.GetName()}");
            }
            Console.WriteLine($"Dependencies:");
            if (Dependencies.Count != 0)
            {
                foreach (IDefinition definition in Dependencies)
                {
                    Console.WriteLine($"    {definition.GetName()}");
                }
            }
            else
            {
                Console.WriteLine($"    <none>");
            }
        }

        protected void AddDependency(IDefinition definition)
        {
            if (!Dependencies.Contains(definition))
            {
                Dependencies.Add(definition);
            }
        }
    }
}
