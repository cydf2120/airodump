#pragma once

#include "dot11.h"
#include "mac.h"

// ----------------------------------------------------------------------------
// GDot11Hdr
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
struct GDot11Hdr : GDot11 {
	Mac addr1_; // receiver address
	Mac addr2_; // transmit address
	Mac addr3_;
	le8_t frag_:4;
	le16_t seq_:12;

	Mac ra() { return addr1_; }
	Mac ta() { return addr2_; }
};
typedef GDot11Hdr *PDot11ExtHdr;
#pragma pack(pop)
