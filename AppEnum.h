#ifndef APPENUM_H
#define APPENUM_H

// do not include anything

/*********************ControllerWorkerThread Enum*********************/
enum class PROXY_TYPE: int
{
    NONE_PROXY = 0,
    TINSOFT_PROXY,
    TMPROXY,
    XPROXY,
};

enum class PROXY_KIND: int
{
    HTTP,
    SOCKS,
};


/*********************NetworkAccessControllerSupporter Enum*********************/
enum class NETWORK_ACCESS_GET_TYPE: int
{
    NOT_SUPPORT=0,
    GET_REQUEST_DEFAULT,
    CHECK_USER_KEY_TINSOFT,
    CHECK_PROXY_KEY_TINSOFT,
    GET_PROXY_FROM_USER_KEY_TINSOFT,
    GET_PROXY_FROM_PROXY_KEY_TINSOFT,
};


enum class NETWORK_ACCESS_POST_TYPE: int
{
    NOT_SUPPORT=0,
    POST_REQUEST_DEFAULT,
    CHECK_USER_KEY_TM,
    GET_PROXY_TM,
};


/*********************DownloadManager Enum*********************/
enum class DOWNLOAD_TYPE: int{
    NONE = 0,
    FILE_VERSION,
    FILE_RELEASE,
};


#endif // APPENUM_H
