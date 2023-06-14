#include <iostream>
#include <algorithm>
#include <vector>
#include <pcap.h>

#include "radiohdr.h"
#include "beaconhdr.h"

struct info {
    Mac         bssid;
    size_t      beacons;
    std::string essid;
};

std::vector<struct info> infoes;

void usage()
{
    std::cout << "syntax : airodump <interface>" << std::endl;
    std::cout << "sample : airodump mon0" << std::endl;
}

void print_info()
{
    system("clear");
    std::cout << "bssid\t\t\tbeacons\t\tessid" << std::endl;
    for (struct info info: infoes)
    {
        std::cout << std::string(info.bssid) << "\t" << info.beacons << "\t\t" << info.essid << std::endl;
    }
}

int main(int argc, char* argv[])
{
    char*               interface;
    char                errbuf[PCAP_ERRBUF_SIZE];
    pcap_t*             handle;
    struct pcap_pkthdr* header;
    const u_char*       packet;
    int                 res;
    GRadioHdr*          radiohdr;
    GBeaconHdr*         beaconhdr;
    Mac                 bssid;
    std::string         essid;
    size_t              beacons;
    bool                dup;
    char                tmp[0x100];

    if (argc != 2)
    {
        usage();
        return 0;
    }

    interface = argv[1];
    handle = pcap_open_live(interface, BUFSIZ, 1, 1, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "couldn't open device %s(%s)\n", interface, errbuf);
        return -1;
    }

    while (true) 
    {
        res = pcap_next_ex(handle, &header, &packet);
        if (res == 0)
            continue;
        if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
            fprintf(stderr, "pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
            break;
        }

        if (header->len < sizeof(GRadioHdr))
            continue;
        
        radiohdr = const_cast<GRadioHdr*>(reinterpret_cast<const GRadioHdr*>(packet));
        if (header->len < radiohdr->len_)
            continue;
        
        beaconhdr = const_cast<GBeaconHdr*>(reinterpret_cast<const GBeaconHdr*>(packet + radiohdr->len_));
        if (beaconhdr->ver_ != 0)
            continue;

        if (beaconhdr->type_ != 0)
            continue;

        if (beaconhdr->subtype_ != GBeaconHdr::Beacon)
            continue;

        bssid = beaconhdr->bssid();
        beacons = 1;

        memset(tmp, 0, sizeof(tmp));
        memcpy(tmp, beaconhdr->firstTag()->value(), beaconhdr->firstTag()->len_);
        essid = std::string(tmp);

        dup = false;
        for (int i = 0; i < infoes.size(); i++) 
        {
            if (infoes[i].bssid == bssid) {
                infoes[i].beacons++;
                dup = true;
                break;
            }
        }

        if (!dup)
            infoes.push_back({bssid, beacons, essid});
        print_info();
    }

}