// https://gitlab.com/gilgil/g/-/blob/master/src/net/pdu/gradiohdr.h

#pragma once

#include <cstdint>

typedef uint8_t le8_t;
typedef uint16_t le16_t;
typedef uint32_t le32_t;
typedef uint64_t le64_t;

// ----------------------------------------------------------------------------
// RadioHdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct GRadioHdr {
	enum PresentFlag : le32_t {
		Tsft = 0,
		Flags = 1,
		Rate = 2,
		Channel = 3,
		Fhss = 4,
		AntennaSignal = 5,
		AntennaNoise = 6,
		LockQuality = 7,
		TxAttenuation = 9,
		DbTxAttenuation = 9,
		DbmTxPower = 10,
		Antenna = 11,
		DbAntennaSignal = 12,
		DbAntennaNoise = 13,
		RxFlags = 14,
		TxFlags = 15,
		RtsRetries = 16,
		DataRetries = 17,
		XChannel = 18,
		Mcs = 19,
		AMpdu = 20,
		Vht = 21,
		Timestamp = 22,
		He = 23,
		HeMu = 24,
		HeMuOtherUser = 25,
		ZeroLenghPsdu = 26,
		LSig = 27,
		Tlv = 28,
		RadiotapNamespace = 29,
		VendorNamespace = 30,
		Ext = 31
	};

	enum Flag : le8_t {
		Cfp = 0x01,
		Preamble = 0x02,
		Wep = 0x04,
		Fragmentation = 0x08,
		FcsAtEnd = 0x10,
		DataPad = 0x20,
		BadFcs = 0x40,
		ShortGi = 0x80
	};

	struct Present {
		Present* next() {
			if ((p_ & 0x80000000) != 0)
				return this + 1;
			return nullptr;
		}

		operator le32_t() { return p_; }

		le32_t p_;
	};

	le8_t ver_;
	le8_t pad_;
	le16_t len_;
	Present present_;
};
typedef GRadioHdr *PRadioHdr;
#pragma pack(pop)
