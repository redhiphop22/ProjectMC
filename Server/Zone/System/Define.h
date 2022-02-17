#pragma once

typedef uint64_t uid_t;
typedef uint32_t map_t;
typedef uint32_t itemID_t;

#define RING_BUFFER_ELEMENT_SIZE 4096

#define MAKE_PACKET_HEADER_C2S(buffer) \
	s2::packet_size_t packetSize = *(s2::packet_size_t*)(&buffer[0]); \
	s2::packet_protocol_t packetProtocol = *(s2::packet_protocol_t*)(&buffer[2]); \
	const char* body = &buffer[6]

#define PACKET_CONVERT_C2S(type, buf, len) \
	flatbuffers::Verifier verifier((const uint8_t*)buf, len); \
	if(false == verifier.VerifyBuffer<protocol::type>()) \
		return false; \
	auto msg = flatbuffers::GetRoot<protocol::type>(buffer);

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

