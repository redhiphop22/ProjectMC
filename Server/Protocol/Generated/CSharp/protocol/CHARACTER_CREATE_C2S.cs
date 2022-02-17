// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace protocol
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct CHARACTER_CREATE_C2S : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static CHARACTER_CREATE_C2S GetRootAsCHARACTER_CREATE_C2S(ByteBuffer _bb) { return GetRootAsCHARACTER_CREATE_C2S(_bb, new CHARACTER_CREATE_C2S()); }
  public static CHARACTER_CREATE_C2S GetRootAsCHARACTER_CREATE_C2S(ByteBuffer _bb, CHARACTER_CREATE_C2S obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CHARACTER_CREATE_C2S __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public string NickName { get { int o = __p.__offset(4); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNickNameBytes() { return __p.__vector_as_span<byte>(4, 1); }
#else
  public ArraySegment<byte>? GetNickNameBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public byte[] GetNickNameArray() { return __p.__vector_as_array<byte>(4); }
  public common.CHARACTER_TYPE Type { get { int o = __p.__offset(6); return o != 0 ? (common.CHARACTER_TYPE)__p.bb.Get(o + __p.bb_pos) : common.CHARACTER_TYPE.NONE; } }
  public ushort Belong { get { int o = __p.__offset(8); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }
  public common.CHARACTER_FACE? Face { get { int o = __p.__offset(10); return o != 0 ? (common.CHARACTER_FACE?)(new common.CHARACTER_FACE()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public common.CHARACTER_EQUIPMENT? Equipment(int j) { int o = __p.__offset(12); return o != 0 ? (common.CHARACTER_EQUIPMENT?)(new common.CHARACTER_EQUIPMENT()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int EquipmentLength { get { int o = __p.__offset(12); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<protocol.CHARACTER_CREATE_C2S> CreateCHARACTER_CREATE_C2S(FlatBufferBuilder builder,
      StringOffset nick_nameOffset = default(StringOffset),
      common.CHARACTER_TYPE type = common.CHARACTER_TYPE.NONE,
      ushort belong = 0,
      Offset<common.CHARACTER_FACE> faceOffset = default(Offset<common.CHARACTER_FACE>),
      VectorOffset equipmentOffset = default(VectorOffset)) {
    builder.StartTable(5);
    CHARACTER_CREATE_C2S.AddEquipment(builder, equipmentOffset);
    CHARACTER_CREATE_C2S.AddFace(builder, faceOffset);
    CHARACTER_CREATE_C2S.AddNickName(builder, nick_nameOffset);
    CHARACTER_CREATE_C2S.AddBelong(builder, belong);
    CHARACTER_CREATE_C2S.AddType(builder, type);
    return CHARACTER_CREATE_C2S.EndCHARACTER_CREATE_C2S(builder);
  }

  public static void StartCHARACTER_CREATE_C2S(FlatBufferBuilder builder) { builder.StartTable(5); }
  public static void AddNickName(FlatBufferBuilder builder, StringOffset nickNameOffset) { builder.AddOffset(0, nickNameOffset.Value, 0); }
  public static void AddType(FlatBufferBuilder builder, common.CHARACTER_TYPE type) { builder.AddByte(1, (byte)type, 0); }
  public static void AddBelong(FlatBufferBuilder builder, ushort belong) { builder.AddUshort(2, belong, 0); }
  public static void AddFace(FlatBufferBuilder builder, Offset<common.CHARACTER_FACE> faceOffset) { builder.AddOffset(3, faceOffset.Value, 0); }
  public static void AddEquipment(FlatBufferBuilder builder, VectorOffset equipmentOffset) { builder.AddOffset(4, equipmentOffset.Value, 0); }
  public static VectorOffset CreateEquipmentVector(FlatBufferBuilder builder, Offset<common.CHARACTER_EQUIPMENT>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateEquipmentVectorBlock(FlatBufferBuilder builder, Offset<common.CHARACTER_EQUIPMENT>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartEquipmentVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<protocol.CHARACTER_CREATE_C2S> EndCHARACTER_CREATE_C2S(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<protocol.CHARACTER_CREATE_C2S>(o);
  }
};


}
