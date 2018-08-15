#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#define MAX_MESSAGE_SIZE 150
#define MAX_MESSAGEB_SIZE 64

#pragma pack(1)

typedef struct HEADER
{
	quint8 id;  /// MESSAGE_ID
	quint64 mac; /// уникальный идентификатор устройства, lk
	quint16 tip; /// идентификатор сообщения
}
    HREADER;

typedef struct PINT
{
    int x;
    int y;
    int z;
}
PINT;

typedef struct POINT2
{
    float x;
    float y;
}
    POINT2;

typedef struct POINT3
{
    float x;
    float y;
    float z;
}
    POINT3;

/// ЮНИКАСТ СООБЩЕНИЯ - ДОСТАВЛЯЮТСЯ АБОНЕНТУ(УСТРОЙСТВУ)

#define TEXTMSG_ID    't'
typedef struct TEXTMSG
{
    HREADER hdr;
    char text[128]; /// текст сообщения для чата

}
    TEXTMSG;

#define COMMAND1_ID    'a'
typedef struct COMMAND1 /// команда с одной координатой, 8-ю параметрами и текстом
{
    HREADER hdr;
    POINT3 cord;
    quint16 params[8];
    char text[96];
}
    COMMAND1;

#define COMMAND2_ID    'b'
typedef struct COMMAND2 /// команда с вектором, 8-ю параметрами и текстом
{
    HREADER hdr;
    POINT3 cords[2];
    quint16 params[8];
    char text[96];
}
    COMMAND2;

#define COMMAND3_ID    'c'
typedef struct COMMAND3 /// команда с набором до 8 точек, 8-ю параметрами и текстом
{
    HREADER hdr;
    POINT2 cords[8];
    quint16 params[8];
    char text[48];
}
    COMMAND3;

#define MSGSTATUS_ID 's'
typedef struct MSGSTATUS /// статус доставки сообщения
{
	HREADER hdr;
	quint8 status; /// 0 - доставлено, другое - не доставлено
}
MSGSTATUS;
/// БРОАДКАСТ СООБЩЕНИЯ - ДОСТАВЛЯЮТСЯ ВСЕМ

#define TELEMETRY_ID    'U'
typedef struct TELEMETRY /// телеметрия
{
    HREADER hdr;

    char name[16]; /// имя KOI8-R
    quint32 mark; /// временная метка, time_t

    uchar state; /// в навигации и др. флаги, бит 0 - признак навигации
    POINT3 pos; /// прямоугольные декартовы координаты или lat, lon, h
    POINT2 dirspd; /// курс скорость

	quint16 id;
}
    TELEMETRY;

typedef struct TELEMETRY_P
{
    char name[16]; /// имя KOI8-R
    quint32 mark; /// временная метка, time_t

    uchar state; /// в навигации и др. флаги, бит 0 - признак навигации
    POINT3 pos; /// прямоугольные декартовы координаты или lat, lon, h
    POINT2 dirspd; /// курс скорость

    quint16 id;
}
    TELEMETRY_P;

#define CONNLOST_ID    'L'
typedef struct CONNLOST /// связь с устройством потеряна
{
    HREADER hdr;
}
    CONNLOST;

#define TEXTMSGB_ID    'T'
typedef struct TEXTMSGB
{
    HREADER hdr;
    char text[48]; /// текст сообщения для чата

}
    TEXTMSGB;

#define COMMAND1B_ID    'A'
typedef struct COMMAND1B ///
{
    HREADER hdr;
    POINT3 cord;
    quint16 params[4];
    char text[32];
}
    COMMAND1B;

#define COMMAND2B_ID    'B'
typedef struct COMMAND2B ///
{
    HREADER hdr;
    POINT3 cords[2];
    quint16 params[4];
    char text[16];
}
    COMMAND2B;

#define COMMAND3B_ID    'C'
typedef struct COMMAND3B ///
{
    HREADER hdr;
    POINT2 cords[6];
    quint16 params[3];
}
    COMMAND3B;

#define IDENTCOMP_ID	'z'

#define DATAEMULATE_T 'Y'
typedef struct DataEmulateTable
{
	HREADER hdr;
	char name[16];
	POINT3 pos;
	POINT2 dirspd;
}
	DataEmulateTable;

#define DELETEEMUL_T 'D'

/// ЮНИКАСТ СООБЩЕНИЯ - ДОСТАВЛЯЮТСЯ АБОНЕНТУ(УСТРОЙСТВУ)

typedef struct DEV_TEXTMSG
{
    char text[128]; /// текст сообщения для чата
}
    DEV_TEXTMSG;

typedef struct DEV_COMMAND1 /// команда с одной координатой, 8-ю параметрами и текстом
{
    POINT3 cord;
    quint16 params[8];
    char text[96];
}
    DEV_COMMAND1;

typedef struct DEV_COMMAND2 /// команда с вектором, 8-ю параметрами и текстом
{
    POINT3 cords[2];
    quint16 params[8];
    char text[96];
}
    DEV_COMMAND2;

typedef struct DEV_COMMAND3 /// команда с набором до 8 точек, 8-ю параметрами и текстом
{
    POINT2 cords[8];
    quint16 params[8];
    char text[48];
}
    DEV_COMMAND3;

typedef struct DEV_MSGSTATUS
{
    quint8 status; /// 0 - доставлено, другое - не доставлено
}
    DEV_MSGSTATUS;

/// БРОАДКАСТ СООБЩЕНИЯ - ДОСТАВЛЯЮТСЯ ВСЕМ

typedef struct DEV_TELEMETRY /// телеметрия
{
    char name[16]; /// имя KOI8-R
    quint32 mark; /// временная метка, time_t

    uchar state; /// 2 - информация о координате не действительна, 1 - информация о координате действительна, 0 - связь потеряна
    POINT3 pos; /// x - широта WGS84 град., y - долгота, h - высота в метрах
    POINT2 dirspd; /// курс скорость
    quint16 id;
}
    DEV_TELEMETRY;

typedef struct DEV_TEXTMSGB
{
    char text[48]; /// текст сообщения для чата
}
    DEV_TEXTMSGB;

typedef struct DEV_COMMAND1B ///
{
    POINT3 cord;
    quint16 params[4];
    char text[32];
}
    DEV_COMMAND1B;

typedef struct DEV_COMMAND2B ///
{
    POINT3 cords[2];
    quint16 params[4];
    char text[16];
}
    DEV_COMMAND2B;

typedef struct DEV_COMMAND3B ///
{
    POINT2 cords[6];
    quint16 params[3];
}
    DEV_COMMAND3B;

#pragma pack()

#endif // _MESSAGES_H_
