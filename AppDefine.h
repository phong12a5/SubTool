#ifndef APPDEFINE_H
#define APPDEFINE_H

/******************************************App Define******************************************/
#ifdef __DEBUG_MODE__
#define APP_VER "1.0.12"
#else
#define APP_VER "1.0.74"
#endif
#define DATABASE_VER "1.00.02"

/*********************SQLite*********************/
#define DATABASE_NAME_DEFAULT "db_maxcare.sqlite"
#define DEFAULT_KEY "Congaubeo@12345Congaubeo@5678910"
#define DEFAULT_IV "Congaubeo@555555"


#define MAX_THREAD_FIELD "config/max_thread"
#define TOKEN_FIELD "config/token"

#define CLONE_INFO_FILED "clone_info/"

#endif // APPDEFINE_H
