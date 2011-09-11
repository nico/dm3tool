#ifndef _DM3_H_
#define _DM3_H_

typedef unsigned char  DM3uint8;
typedef unsigned short DM3uint16;
typedef unsigned int   DM3uint32;

#pragma pack(push, 1)


#define DM3_DEF_SHORT   2
#define DM3_DEF_LONG    3
#define DM3_DEF_USHORT  4
#define DM3_DEF_ULONG   5
#define DM3_DEF_FLOAT   6
#define DM3_DEF_DOUBLE  7
#define DM3_DEF_BOOL    8
#define DM3_DEF_CHAR    9  // single 8bit char
#define DM3_DEF_OCTET  10
#define DM3_DEF_STRUCT 15
#define DM3_DEF_STRING 18  // utf16 data
#define DM3_DEF_ARRAY  20

// DM3_DEF_STRUCT is followed by:
//   struct_name_len
//   num_fields
//   (field_name_len, field_type) x num_fields
//
// DM3_DEF_STRING is followed by:
//   string_length
//
// DM3_DEF_ARRAY is followed by:
//   array_type
//   array_length

struct DM3TagData {
  DM3uint8 tag[4];  // Always '%%%%'.
  DM3uint32 definition_length;
  DM3uint32 definition[];
  // Followed by the actual data.
};

#define DM3_TAG_ENTRY_TYPE_TAG_GROUP 20
#define DM3_TAG_ENTRY_TYPE_DATA      21

struct DM3TagEntry {
  DM3uint8 type;
  DM3uint16 label_length;
  DM3uint8 label[];
  // Followed by a DM3TagGroup if |type == DM3_TAG_ENTRY_TYPE_TAG_GROUP|
  // or a DM3TagData if |type == DM3_TAG_ENTRY_TYPE_DATA|.
};

struct DM3TagGroup {
  DM3uint8 is_sorted;
  DM3uint8 is_open;
  DM3uint32 num_tags;
  struct DM3TagEntry tags[];
};

struct DM3Image {
  DM3uint32 version;
  DM3uint32 length;
  DM3uint32 is_little_endian;  // This really means something else.
  struct DM3TagGroup tag_group;
};

#pragma pack(pop)

#endif  // _DM3_H_
