namespace Hedron.Definition
{
    public enum DefinitionValidationMessageType
    {
        Info,
        Warning,
        Error,
    }

    public class DefinitionValidationMessage
    {
        public DefinitionValidationMessageType MessageType { get; set; }
        public IDefinition Definition;

        public DefinitionValidationMessage(DefinitionValidationMessageType messageType, IDefinition definition)
        {
            MessageType = messageType;
            Definition = definition;
        }
    }
}
