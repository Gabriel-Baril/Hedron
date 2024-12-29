using Hedron.Feature;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hedron.Feature
{
    [ArchFeature(Version: 0, Name: "filefeature")]
    public abstract class IFileFeature : IFeature
    {
        // TODO: public abstract IFilePathFeature FilePath(); // For example, feature_output_path. You could have a chain of IFilePathFeature

        public abstract string FilePath();

        public override byte[] SerializeData()
        {
            return null;
        }
    }
}
