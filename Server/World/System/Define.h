#pragma once

typedef uint64_t uid_t;
typedef uint32_t map_t;
typedef uint32_t itemID_t;

#define MAKE_PACKET_HEADER_C2S(buffer) \
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&buffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&buffer[2]); \
	const char* body = &buffer[6]

#define PACKET_CONVERT_C2S(recv, send, buf, len) \
	auto SendError = [session](auto result) ->void { \
		flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE); \
		auto body = protocol::Create##send(fbb, result); \
		fbb.Finish(body); \
		session->SendPacket(protocol::MESSAGE_##send, fbb); \
	}; \
	flatbuffers::Verifier verifier((const uint8_t*)buf, len); \
	if(false == verifier.VerifyBuffer<protocol::recv>()) { \
		SendError(common::RESULT_CODE_ERROR_PACKET_VERIFIER); \
		return false; \
	}\
	uid_t uid = session->GetUID();\
	auto msg = flatbuffers::GetRoot<protocol::recv>(buffer);

#define MAKE_PACKET_HEADER_S2S(buffer) \
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&buffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&buffer[2]); \
	const char* body = &buffer[4]

#define PACKET_CONVERT_S2S(type, buf, len) \
	flatbuffers::Verifier verifier((const uint8_t*)buf, len); \
	if(false == verifier.VerifyBuffer<protocol_svr::type>()) \
		return false; \
	auto msg = flatbuffers::GetRoot<protocol_svr::type>(buffer);

#define REGIST_PACKET(p) { auto result = p; \
	if(false == result.second) return false; }

