#pragma once

class IocpSession;

class UserInfo
{
public:
	struct CharacterFace
	{
		uint16_t	m_hairIdx;			// Çì¾î
		uint8_t		m_hairColor;
		uint16_t	m_faceIdx;			// ¾ó±¼
		uint8_t		m_faceColor;
		uint16_t	m_eyeIdx;			// ´«µ¿ÀÚ
		uint8_t		m_eyeColor;
		uint16_t	m_eyebrowIdx;		// ´«½ç
		uint8_t		m_eyebrowColor;
		uint16_t	m_noseIdx;			// ÄÚ
		uint8_t		m_noseColor;
		uint16_t	m_mouthIdx;			// ÀÔ¼ú
		uint8_t		m_mouthColor;

		flatbuffers::Offset<common::CHARACTER_FACE> Packing(flatbuffers::FlatBufferBuilder& fbb)
		{
			return common::CreateCHARACTER_FACE(fbb,
				m_hairIdx,
				m_hairColor,
				m_faceIdx,
				m_faceColor,
				m_eyeIdx,
				m_eyeColor,
				m_eyebrowIdx,
				m_eyebrowColor,
				m_noseIdx,
				m_noseColor,
				m_mouthIdx,
				m_mouthColor
			);
		}
		void UnPacking(const common::CHARACTER_FACE& info)
		{
			m_hairIdx = info.hair_idx();
			m_hairColor = info.hair_color();
			m_faceIdx = info.face_idx();
			m_faceColor = info.face_color();
			m_eyeIdx = info.eye_idx();
			m_eyeColor = info.eye_color();
			m_eyebrowIdx = info.eyebrow_idx();
			m_eyebrowColor = info.eyebrow_color();
			m_noseIdx = info.nose_idx();
			m_noseColor = info.nose_color();
			m_mouthIdx = info.mouth_idx();
			m_mouthColor = info.mouth_color();
		}
	};

	struct CharacterEquipmentList
	{
		std::unordered_map<common::CHARACTER_EQUIPMENT_TYPE, uint32_t> m_equipmentInfo;

		std::vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>> Packing(flatbuffers::FlatBufferBuilder& fbb)
		{
			std::vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>> equipment;
			for(auto equip : m_equipmentInfo)
			{
				equipment.emplace_back(
					common::CreateCHARACTER_EQUIPMENT(fbb,
						equip.first,
						equip.second
					)
				);
			}
			return equipment;
		}
		void UnPacking(const flatbuffers::Vector<flatbuffers::Offset<common::CHARACTER_EQUIPMENT>>* info)
		{
			for(uint32_t i = 0 ; i < info->size() ; ++i)
			{
				auto equipment = info->Get(i);
				m_equipmentInfo.emplace(equipment->type(), equipment->id());
			}
		}
	};

public:
	UserInfo(IocpSession* session, uid_t uid, common::ACCOUNT_AUTHORITY authority);
	~UserInfo();

	void					Destroy();

	IocpSession*			GetSession()				{	return m_session;	}
	bool					SendPacket(protocol::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb);

public:
	bool					CHARACTER_INFO_DB_ACK(const protocol_svr::CHARACTER_INFO_DB_ACK* msg);
	bool					CHARACTER_CREATE_DB_ACK(const protocol_svr::CHARACTER_CREATE_DB_ACK* msg);

protected:

private:
	IocpSession*			m_session = nullptr;

	std::string				m_nickName;
	common::CHARACTER_TYPE	m_characterType;
	uint32_t				m_belong;

	CharacterFace			m_faceInfo;
	CharacterEquipmentList	m_equipmentList;
};