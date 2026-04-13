
#ifndef HEADER_GUARD_OSGFFMPEG_FFMPEG_PACKET_H
#define HEADER_GUARD_OSGFFMPEG_FFMPEG_PACKET_H

#include "FFmpegHeaders.hpp"



namespace osgFFmpeg
{

    struct FFmpegPacket
    {

        enum Type
        {
            PACKET_DATA,
            PACKET_END_OF_STREAM,
            PACKET_FLUSH
        };


        FFmpegPacket() :
            type(PACKET_DATA)
        {
            av_init_packet(&packet);
            packet.data = 0;
            packet.size = 0;
        }

        explicit FFmpegPacket(const Type t) :
            type(t)
        {
            av_init_packet(&packet);
            packet.data = 0;
            packet.size = 0;
        }

        explicit FFmpegPacket(const AVPacket & p) :
            type(PACKET_DATA)
        {
            av_init_packet(&packet);
            packet.data = 0;
            packet.size = 0;
            av_packet_ref(&packet, &p);
        }

        FFmpegPacket(const FFmpegPacket& rhs) :
            type(rhs.type)
        {
            av_init_packet(&packet);
            packet.data = 0;
            packet.size = 0;

            if (rhs.packet.data != 0 || rhs.packet.size != 0)
                av_packet_ref(&packet, &rhs.packet);
        }

        FFmpegPacket& operator=(const FFmpegPacket& rhs)
        {
            if (this == &rhs) return *this;

            clear();
            type = rhs.type;

            if (rhs.packet.data != 0 || rhs.packet.size != 0)
                av_packet_ref(&packet, &rhs.packet);

            return *this;
        }

        ~FFmpegPacket()
        {
            clear();
        }

        void clear()
        {
            av_packet_unref(&packet);
            type = PACKET_DATA;
        }

        void release()
        {
            av_packet_unref(&packet);
            type = PACKET_DATA;
        }

        bool valid() const
        {
            return (type != PACKET_DATA) ^ (packet.data != 0);
        }

        bool operator ! () const
        {
            return ! valid();
        }

        AVPacket    packet;
        Type        type;
    };

    struct FFmpegPacketClear
    {
        void operator () (FFmpegPacket & packet) const
        {
            packet.clear();
        }
    };

}



#endif // HEADER_GUARD_OSGFFMPEG_FFMPEG_PACKET_H
