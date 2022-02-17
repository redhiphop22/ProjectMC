// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace common
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct ENTITY_INFO : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static ENTITY_INFO GetRootAsENTITY_INFO(ByteBuffer _bb) { return GetRootAsENTITY_INFO(_bb, new ENTITY_INFO()); }
  public static ENTITY_INFO GetRootAsENTITY_INFO(ByteBuffer _bb, ENTITY_INFO obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public ENTITY_INFO __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ulong Uid { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUlong(o + __p.bb_pos) : (ulong)0; } }
  public string NickName { get { int o = __p.__offset(6); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNickNameBytes() { return __p.__vector_as_span<byte>(6, 1); }
#else
  public ArraySegment<byte>? GetNickNameBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public byte[] GetNickNameArray() { return __p.__vector_as_array<byte>(6); }
  public common.CHARACTER_FACE? Face { get { int o = __p.__offset(8); return o != 0 ? (common.CHARACTER_FACE?)(new common.CHARACTER_FACE()).__assign(__p.__indirect(o + __p.bb_pos), __p.bb) : null; } }
  public common.CHARACTER_EQUIPMENT? Equipment(int j) { int o = __p.__offset(10); return o != 0 ? (common.CHARACTER_EQUIPMENT?)(new common.CHARACTER_EQUIPMENT()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int EquipmentLength { get { int o = __p.__offset(10); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<common.ENTITY_INFO> CreateENTITY_INFO(FlatBufferBuilder builder,
      ulong uid = 0,
      StringOffset nick_nameOffset = default(StringOffset),
      Offset<common.CHARACTER_FACE> faceOffset = default(Offset<common.CHARACTER_FACE>),
      VectorOffset equipmentOffset = default(VectorOffset)) {
    builder.StartTable(4);
    ENTITY_INFO.AddUid(builder, uid);
    ENTITY_INFO.AddEquipment(builder, equipmentOffset);
    ENTITY_INFO.AddFace(builder, faceOffset);
    ENTITY_INFO.AddNickName(builder, nick_nameOffset);
    return ENTITY_INFO.EndENTITY_INFO(builder);
  }

  public static void StartENTITY_INFO(FlatBufferBuilder builder) { builder.StartTable(4); }
  public static void AddUid(FlatBufferBuilder builder, ulong uid) { builder.AddUlong(0, uid, 0); }
  public static void AddNickName(FlatBufferBuilder builder, StringOffset nickNameOffset) { builder.AddOffset(1, nickNameOffset.Value, 0); }
  public static void AddFace(FlatBufferBuilder builder, Offset<common.CHARACTER_FACE> faceOffset) { builder.AddOffset(2, faceOffset.Value, 0); }
  public static void AddEquipment(FlatBufferBuilder builder, VectorOffset equipmentOffset) { builder.AddOffset(3, equipmentOffset.Value, 0); }
  public static VectorOffset CreateEquipmentVector(FlatBufferBuilder builder, Offset<common.CHARACTER_EQUIPMENT>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateEquipmentVectorBlock(FlatBufferBuilder builder, Offset<common.CHARACTER_EQUIPMENT>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartEquipmentVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<common.ENTITY_INFO> EndENTITY_INFO(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<common.ENTITY_INFO>(o);
  }
};


}
