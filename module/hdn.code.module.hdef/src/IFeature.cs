using Google.FlatBuffers;
using HedronFeatureDescriptor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Hedron.Feature
{
    public static class IFeatureUtil
    {
        public delegate VectorOffset CreateDependencyVectorFunc(FlatBufferBuilder builder, Offset<Hedron.Feature.FeatureSignature>[] list);

        public static Offset<FeatureMetadata> ConstructFeatureMetadata(FlatBufferBuilder builder, IFeature feature)
        {
            var archTypeOffset = builder.CreateString(feature.GetType().BaseType?.Name);
            var typeChainOffset = builder.CreateString(TypeUtil.GetTypeChain(feature));
            FeatureMetadata.StartFeatureMetadata(builder);

            FeatureAttribute? attribute = (FeatureAttribute?)feature.GetType().GetCustomAttribute(typeof(FeatureAttribute), false);
            if(attribute != null)
            {
                FeatureMetadata.AddVersion(builder, attribute.Version);
            }
            else
            {
                FeatureMetadata.AddVersion(builder, 0);
            }

            FeatureMetadata.AddArchType(builder, archTypeOffset);
            FeatureMetadata.AddTypeChain(builder, typeChainOffset);
            return FeatureMetadata.EndFeatureMetadata(builder);
        }

        public static Offset<FeatureSignature> ConstructFeatureSignature(FlatBufferBuilder builder, IFeature? feature)
        {
            string featureName = string.Empty;
            if(feature != null)
            {
                featureName = feature.GetName();
            }

            var nameOffset = builder.CreateString(featureName);
            FeatureSignature.StartFeatureSignature(builder);
            FeatureSignature.AddName(builder, nameOffset);
            return FeatureSignature.EndFeatureSignature(builder);
        }

        public static VectorOffset ConstructFeatureSignatureList(FlatBufferBuilder builder, List<IFeature> featureList, CreateDependencyVectorFunc createDependencyVectorFunc)
        {
            var dependenciesOffsets = new List<Offset<FeatureSignature>>();
            foreach (IFeature feature in featureList)
            {
                var offset = ConstructFeatureSignature(builder, feature);
                dependenciesOffsets.Add(offset);
            }
            return createDependencyVectorFunc(builder, dependenciesOffsets.ToArray());
        }
    }

    public abstract class IFeature
    {
        public bool Constructed { get; set; } = false;
        public List<IFeature> Dependencies { get; set; } = new List<IFeature>();
        IParameterBinder? Binder { get; set; } = null;

        public IFeature()
        {
        }

        // Used by the feature compiler
        public void SetBinderFromType(Type binderType)
        {
            Binder = (IParameterBinder?)Activator.CreateInstance(binderType);
        }

        public dynamic GetParam(string ParamName, dynamic Default)
        {
            if(Binder == null)
            {
                return Default;
            }
            Binder.Construct();
            return Binder.Get(ParamName, Default);
        }

        public void SaveAll(string path)
        {
            Save(path);
            foreach (IFeature feature in Dependencies)
            {
                feature.Binder = (feature is IParameterBinder) ? null : Binder;
                feature.SaveAll(path);
            }
        }

        public void Save(string outFolder)
        {
            Construct();
            string featureName = GetName();

            List<FeatureValidationMessage> validationMessages = Validate();
            foreach (FeatureValidationMessage validationMessage in validationMessages)
            {
                Console.WriteLine($"[{validationMessage.MessageType}][{featureName}]: {validationMessage}");
            }
            Console.WriteLine($"Writing to {outFolder}{featureName}.feature");
            File.WriteAllBytes($"{outFolder}{featureName}.feature", Serialize());
        }

        public byte[] Serialize()
        {
            FlatBufferBuilder builder = new FlatBufferBuilder(1024);
            var metadataOffset = IFeatureUtil.ConstructFeatureMetadata(builder, this);
            var signatureOffset = IFeatureUtil.ConstructFeatureSignature(builder, this);
            var binderOffset = IFeatureUtil.ConstructFeatureSignature(builder, Binder);
            var dependenciesOffset = IFeatureUtil.ConstructFeatureSignatureList(builder, Dependencies, Feature.CreateDependenciesVector);
            var dataOffset = Feature.CreateDataVector(builder, SerializeData());
            Feature.StartFeature(builder);
            Feature.AddMetadata(builder, metadataOffset);
            Feature.AddSignature(builder, signatureOffset);
            Feature.AddBinder(builder, binderOffset);
            Feature.AddDependencies(builder, dependenciesOffset);
            Feature.AddData(builder, dataOffset);
            var featureOffset = Feature.EndFeature(builder);
            builder.Finish(featureOffset.Value);
            return builder.SizedByteArray();
        }

        public virtual List<FeatureValidationMessage> Validate()
        {
            return new List<FeatureValidationMessage>();
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
            if(Binder != null)
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
            if(Binder != null)
            {
                Console.WriteLine($"Binder: {Binder.GetName()}");
            }
            Console.WriteLine($"Dependencies:");
            if(Dependencies.Count != 0)
            {
                foreach (IFeature feature in Dependencies)
                {
                    Console.WriteLine($"    {feature.GetName()}");
                }
            }
            else
            {
                Console.WriteLine($"    <none>");
            }
        }

        protected void AddDependency(IFeature feature)
        {
            if(!Dependencies.Contains(feature))
            {
                Dependencies.Add(feature);
            }
        }
    }
}
