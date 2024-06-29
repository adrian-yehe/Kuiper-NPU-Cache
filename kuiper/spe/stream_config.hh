#ifndef __STREAM_CONFIG__H__
#define __STREAM_CONFIG__H__

#include <string>
#include <stdint.h>
#include <array>
namespace gem5 
{
    namespace kuiper 
    {
        enum class stream_type_e 
        {
            SSU = 0,
            DIRECT = 1,
            MAX_TYPE
        };
        using STREAM_TYPE = enum stream_type_e;
        const std::array<std::string, static_cast<std::uint8_t>(STREAM_TYPE::MAX_TYPE)> stream_type_name = 
        {
            "ssu_stream", "direct_stream"            
        };

        const std::array<std::string,7> stream_attr_name = 
        {
            "unicast", 
            "scatter",
            "multcast",
            "point_to_point", 
            "gather",
            "all_to_all",
            "all_to_gather",
        };

        struct  linked_info_t
        {
            /* linked information */
            std::uint32_t tile_id;
            std::uint64_t stream_id;
            std::uint32_t attribute;
        };

        struct stream_info_t
        {
            /* stream configuration comes from compiler generation */
            std::uint64_t stream_id;
            std::uint32_t stream_type;
            std::uint32_t attribute;
            std::uint32_t packet_cnt; /* value less than or equal to 8 */
            std::uint32_t packet_size;
            std::uint32_t link_stream_count; 
        };

        struct instream_linked_info_t
        {
            /* data */
            struct  linked_info_t linked_info;
            std::uint32_t packet_count; /* value less than or equal to 8 */
            std::uint32_t packet_size;
        };

        struct instream_info_t
        {
            struct stream_info_t stream_info; 
            std::uint32_t link_stream_count; 
        };

        struct outstream_linked_info_t
        {
            /* outstream link information data between witch instream */
            struct  linked_info_t linked_info;
        };

        struct outstream_info_t
        {
            /* out stream information ,which stores in 3DRAM */
            struct stream_info_t stream_info; 
            std::uint32_t link_stream_count; 
        };
    }
}

#endif /* __STREAM_CONFIG__H__ */
