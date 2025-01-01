// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_DEFINITION_HEDRON_DEFINITION_H_
#define FLATBUFFERS_GENERATED_DEFINITION_HEDRON_DEFINITION_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace Hedron {
namespace Definition {

struct DefinitionMetadata;
struct DefinitionMetadataBuilder;

struct DefinitionSignature;
struct DefinitionSignatureBuilder;

struct Definition;
struct DefinitionBuilder;

struct DefinitionMetadata FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef DefinitionMetadataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VERSION = 4,
    VT_ARCH_TYPE = 6,
    VT_TYPE_CHAIN = 8
  };
  uint32_t version() const {
    return GetField<uint32_t>(VT_VERSION, 0);
  }
  const ::flatbuffers::String *arch_type() const {
    return GetPointer<const ::flatbuffers::String *>(VT_ARCH_TYPE);
  }
  const ::flatbuffers::String *type_chain() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TYPE_CHAIN);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_VERSION, 4) &&
           VerifyOffset(verifier, VT_ARCH_TYPE) &&
           verifier.VerifyString(arch_type()) &&
           VerifyOffset(verifier, VT_TYPE_CHAIN) &&
           verifier.VerifyString(type_chain()) &&
           verifier.EndTable();
  }
};

struct DefinitionMetadataBuilder {
  typedef DefinitionMetadata Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_version(uint32_t version) {
    fbb_.AddElement<uint32_t>(DefinitionMetadata::VT_VERSION, version, 0);
  }
  void add_arch_type(::flatbuffers::Offset<::flatbuffers::String> arch_type) {
    fbb_.AddOffset(DefinitionMetadata::VT_ARCH_TYPE, arch_type);
  }
  void add_type_chain(::flatbuffers::Offset<::flatbuffers::String> type_chain) {
    fbb_.AddOffset(DefinitionMetadata::VT_TYPE_CHAIN, type_chain);
  }
  explicit DefinitionMetadataBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<DefinitionMetadata> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<DefinitionMetadata>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<DefinitionMetadata> CreateDefinitionMetadata(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t version = 0,
    ::flatbuffers::Offset<::flatbuffers::String> arch_type = 0,
    ::flatbuffers::Offset<::flatbuffers::String> type_chain = 0) {
  DefinitionMetadataBuilder builder_(_fbb);
  builder_.add_type_chain(type_chain);
  builder_.add_arch_type(arch_type);
  builder_.add_version(version);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<DefinitionMetadata> CreateDefinitionMetadataDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t version = 0,
    const char *arch_type = nullptr,
    const char *type_chain = nullptr) {
  auto arch_type__ = arch_type ? _fbb.CreateString(arch_type) : 0;
  auto type_chain__ = type_chain ? _fbb.CreateString(type_chain) : 0;
  return Hedron::Definition::CreateDefinitionMetadata(
      _fbb,
      version,
      arch_type__,
      type_chain__);
}

struct DefinitionSignature FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef DefinitionSignatureBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4
  };
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           verifier.EndTable();
  }
};

struct DefinitionSignatureBuilder {
  typedef DefinitionSignature Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(DefinitionSignature::VT_NAME, name);
  }
  explicit DefinitionSignatureBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<DefinitionSignature> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<DefinitionSignature>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<DefinitionSignature> CreateDefinitionSignature(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0) {
  DefinitionSignatureBuilder builder_(_fbb);
  builder_.add_name(name);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<DefinitionSignature> CreateDefinitionSignatureDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return Hedron::Definition::CreateDefinitionSignature(
      _fbb,
      name__);
}

struct Definition FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef DefinitionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_METADATA = 4,
    VT_SIGNATURE = 6,
    VT_DEPENDENCIES = 8,
    VT_DATA = 10
  };
  const Hedron::Definition::DefinitionMetadata *metadata() const {
    return GetPointer<const Hedron::Definition::DefinitionMetadata *>(VT_METADATA);
  }
  const Hedron::Definition::DefinitionSignature *signature() const {
    return GetPointer<const Hedron::Definition::DefinitionSignature *>(VT_SIGNATURE);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<Hedron::Definition::DefinitionSignature>> *dependencies() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<Hedron::Definition::DefinitionSignature>> *>(VT_DEPENDENCIES);
  }
  const ::flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const ::flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_METADATA) &&
           verifier.VerifyTable(metadata()) &&
           VerifyOffset(verifier, VT_SIGNATURE) &&
           verifier.VerifyTable(signature()) &&
           VerifyOffset(verifier, VT_DEPENDENCIES) &&
           verifier.VerifyVector(dependencies()) &&
           verifier.VerifyVectorOfTables(dependencies()) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct DefinitionBuilder {
  typedef Definition Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_metadata(::flatbuffers::Offset<Hedron::Definition::DefinitionMetadata> metadata) {
    fbb_.AddOffset(Definition::VT_METADATA, metadata);
  }
  void add_signature(::flatbuffers::Offset<Hedron::Definition::DefinitionSignature> signature) {
    fbb_.AddOffset(Definition::VT_SIGNATURE, signature);
  }
  void add_dependencies(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Hedron::Definition::DefinitionSignature>>> dependencies) {
    fbb_.AddOffset(Definition::VT_DEPENDENCIES, dependencies);
  }
  void add_data(::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(Definition::VT_DATA, data);
  }
  explicit DefinitionBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Definition> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Definition>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Definition> CreateDefinition(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<Hedron::Definition::DefinitionMetadata> metadata = 0,
    ::flatbuffers::Offset<Hedron::Definition::DefinitionSignature> signature = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Hedron::Definition::DefinitionSignature>>> dependencies = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> data = 0) {
  DefinitionBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_dependencies(dependencies);
  builder_.add_signature(signature);
  builder_.add_metadata(metadata);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Definition> CreateDefinitionDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<Hedron::Definition::DefinitionMetadata> metadata = 0,
    ::flatbuffers::Offset<Hedron::Definition::DefinitionSignature> signature = 0,
    const std::vector<::flatbuffers::Offset<Hedron::Definition::DefinitionSignature>> *dependencies = nullptr,
    const std::vector<uint8_t> *data = nullptr) {
  auto dependencies__ = dependencies ? _fbb.CreateVector<::flatbuffers::Offset<Hedron::Definition::DefinitionSignature>>(*dependencies) : 0;
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return Hedron::Definition::CreateDefinition(
      _fbb,
      metadata,
      signature,
      dependencies__,
      data__);
}

inline const Hedron::Definition::Definition *GetDefinition(const void *buf) {
  return ::flatbuffers::GetRoot<Hedron::Definition::Definition>(buf);
}

inline const Hedron::Definition::Definition *GetSizePrefixedDefinition(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<Hedron::Definition::Definition>(buf);
}

inline bool VerifyDefinitionBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Hedron::Definition::Definition>(nullptr);
}

inline bool VerifySizePrefixedDefinitionBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Hedron::Definition::Definition>(nullptr);
}

inline void FinishDefinitionBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Hedron::Definition::Definition> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedDefinitionBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Hedron::Definition::Definition> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Definition
}  // namespace Hedron

#endif  // FLATBUFFERS_GENERATED_DEFINITION_HEDRON_DEFINITION_H_
